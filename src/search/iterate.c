#include "prophet/search.h"

#include "prophet/const.h"
#include "prophet/movegen.h"
#include "prophet/search.h"

#include "search_internal.h"
#include "hash/hash_internal.h"
#include "movegen/movegen_internal.h"
#include "util/output.h"
#include "util/string_utils.h"
#include "util/time.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t volatile max_depth = 0;
uint32_t volatile max_time_ms = 0;
uint32_t volatile hash_age = 0;


extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern move_line_t last_pv;
extern hash_table_t htbl;
extern hash_table_t phtbl;
extern bool volatile stop_search;
extern bool volatile skip_time_checks;
extern move_t moves[MAX_PLY * MAX_MOVES_PER_PLY];

/* forward decls */
static void print_search_summary(int32_t last_depth, uint64_t start_time, const stats_t* stats);
static bool best_at_top(move_t* start, move_t* end);


/* Currently only being used for fixed depth testing */
int iterate_from_fen(const char *fen, move_t* pv, int* n, uint64_t* nodes, uint64_t* qnodes, int depth, pv_func_t pv_callback) {
    int retval = 0;

    position_t pos;
    if (!set_pos(&pos, fen)) return 1; /* TODO: error code */

    /* set up the options */
    iterator_options_t* opts = (iterator_options_t*)malloc(sizeof(iterator_options_t));
    memset(opts, 0, sizeof(iterator_options_t));
    opts->early_exit_ok = false; /* TODO: if not fixed time per move */
    opts->max_depth = depth;
    opts->max_time_ms = 0;
    opts->post_mode = false;
    opts->pv_callback = pv_callback;
    opts->clear_hash_each_search = false;

    /* TODO: replay move history.  verify move_counter and fifty_counter */
    iterator_context_t* ctx = (iterator_context_t*)malloc(sizeof(iterator_context_t));
    ctx->pos = &pos;
    ctx->move_stack = moves;
    ctx->undo_stack = gundos;

    stats_t stats;
    memset(&stats, 0, sizeof(stats_t));

    move_line_t pv_line = iterate(opts, ctx, &stats);

    /* copy the PV into the return structure */
    memcpy(pv, pv_line.mv, sizeof(move_t) * pv_line.n);
    *n = pv_line.n;

    /* record the search stats */
    *nodes = stats.nodes;
    *qnodes = stats.qnodes;

    /* clean up */
    free(ctx);
    free(opts);

    /* success */
    return retval;
}

/**
 * \brief Search the position using iterative deepening. 
 * 
 * \param opts          the options structure
 * \pram ctx            the context for this search iterator
 *
 * \return the principal variation
 */ 
move_line_t iterate(const iterator_options_t* opts, const iterator_context_t* ctx, stats_t *stats)
{
    move_line_t pv; pv.n = 0;

    /* generate and count the number of moves to choose from */
    move_t* endp = gen_legal_moves(ctx->move_stack, ctx->pos, true, true);
    int num_caps, num_noncaps;
    num_moves_in_list(ctx->move_stack, endp, &num_caps, &num_noncaps);

    /* initialize the PV to ensure we have a valid move to play */
    best_at_top(ctx->move_stack, endp);
    pv.mv[0] = ctx->move_stack[0];
    pv.n = 1;

    /* if just one legal move, don't bother searching */
    if (opts->early_exit_ok && num_caps + num_noncaps == 1) {
        return pv;
    }

    /* set up options */
    search_options_t search_opts;
    memset(&search_opts, 0, sizeof(search_options_t));
    search_opts.pv_callback = opts->pv_callback;
    search_opts.start_time = milli_timer();
    if (opts->max_time_ms) {
        search_opts.stop_time = search_opts.start_time + opts->max_time_ms;
        search_opts.nodes_between_time_checks = 100000UL;
        /* if we're getting low on time, check more often */
        if (opts->max_time_ms < 10000) {
            search_opts.nodes_between_time_checks /= 10;
        }
        if (opts->max_time_ms < 1000) {
            search_opts.nodes_between_time_checks /= 10;   
        }
    }

    /* prepare to search */
    memset(&last_pv, 0, sizeof(move_line_t));
    uint32_t depth = 0;
    int32_t score = 0;
    hash_age++;

    /* search using iterative deepening */
    bool stop_iterator = false;
    do {
        ++depth;

        /* clear the hash, if that option is set.  this is mainly used for debugging. */
        if (opts->clear_hash_each_search) {
            clear_hash_table(&htbl);
        }

        /* set up the search */
        int32_t alpha_bound = -CHECKMATE;
        int32_t beta_bound = CHECKMATE;
        move_line_t search_pv; search_pv.n = 0;

        /* start the search */
        score = search(ctx->pos, &search_pv, depth, alpha_bound, beta_bound, 
            ctx->move_stack, ctx->undo_stack, stats, &search_opts);

        /* If the search returned a PV, we can use it since the last iteration's PV was tried first */
        if (search_pv.n > 0) {
            memcpy(&pv, &search_pv, sizeof(move_line_t));
        }

        if (stop_search) {
            break;
        }

        /* print the move line */
        uint64_t elapsed = milli_timer() - search_opts.start_time;
        if (opts->pv_callback) {
            opts->pv_callback(&(pv.mv[0]), pv.n, depth, score, elapsed, stats->nodes);
        }

        /* if the search discovered a checkmate, stop. */
        if (abs(score) > (CHECKMATE - 500)) {
            stop_iterator = true;
        }

        /* if the search has reached the max user defined  depth, stop */
        if (opts->max_depth > 0 && depth >= opts->max_depth) {
            stop_iterator = true;
        }

        /* if we've hit the max system defined depth, stop */
        if (depth >= MAX_ITERATIONS) {
            stop_iterator = true;
        }

        /* if we've used more than half our time, don't start a new 
        *  iteration 
        */
        if (opts->early_exit_ok && !skip_time_checks && (elapsed > opts->max_time_ms / 2)) {
            stop_iterator = true;
        }

    } while (!stop_iterator);

    assert(pv.n > 0);

    /* print the search summary */
    /* TODO: return stats and print outside of this function */
    if (opts->post_mode) {
        print_search_summary(depth, search_opts.start_time, stats);
    }

    return pv;
}


static void print_search_summary(int32_t last_depth, uint64_t start_time, const stats_t* stats)
{
    plog("\n");
    plog("# depth: %d\n", last_depth);

    /* print node counts */
    uint64_t total_nodes = stats->nodes + stats->qnodes;

    float interior_pct = stats->nodes / (total_nodes/100.0);
    float qnode_pct = stats->qnodes / (total_nodes/100.0);    

    plog( 
        "# nodes: %lluk, interior: %lluk (%.2f%%), quiescence: %lluk (%.2f%%)\n",
        total_nodes/1000, stats->nodes/1000, interior_pct, stats->qnodes/1000, 
        qnode_pct);

    /* calculate the search time.  add 1 ms just to avoid div by 0 */
    uint64_t search_time_ms = milli_timer() - start_time + 1; 
    float search_time = search_time_ms / 1000.0;
    uint64_t nps = total_nodes / search_time_ms;
    plog("# search time: %.2f seconds, rate: %llu kn/s\n",
        search_time,nps);

    /* display hash stats */
    uint64_t hash_hits = htbl.hits;
    uint64_t hash_probes = htbl.probes;
    uint64_t hash_collisions = htbl.collisions;
    float hash_hit_pct = hash_hits / (hash_probes/100.0);
    float hash_collision_pct = hash_collisions / (hash_probes/100.0);
    plog("# hash probes: %lluk, hits: %lluk (%.2f%%), "
        "collisions: %lluk (%.2f%%)\n",
        hash_probes/1000,
        hash_hits/1000, hash_hit_pct,
        hash_collisions/1000, hash_collision_pct);

    /* display pawn hash stats */
    uint64_t pawn_hash_hits = phtbl.hits;
    uint64_t pawn_hash_probes = phtbl.probes;
    uint64_t pawn_hash_collisions = phtbl.collisions;
    float pawn_hash_hit_pct = pawn_hash_hits / (pawn_hash_probes/100.0);
    float pawn_hash_collision_pct = pawn_hash_collisions / (pawn_hash_probes/100.0);
    plog("# pawn hash probes: %lluk, hits: %lluk (%.2f%%), "
        "collisions: %lluk (%.2f%%)\n",
        pawn_hash_probes/1000,
        pawn_hash_hits/1000, pawn_hash_hit_pct,
        pawn_hash_collisions/1000, pawn_hash_collision_pct);

    /* fail high metrics */
    float hash_fail_high_pct = stats->hash_fail_highs / (hash_probes/100.0);
    float hash_fail_low_pct = stats->hash_fail_lows / (hash_probes/100.0);
    float hash_exact_score_pct = stats->hash_exact_scores / (hash_probes/100.0);

    plog("# hash fail highs: %lluk (%.2f%%), "
        "fail lows: %lluk (%.2f%%), exact scores: %lluk (%.2f%%)\n",
        stats->fail_highs, hash_fail_high_pct,
        stats->fail_lows, hash_fail_low_pct,
        stats->hash_exact_scores, hash_exact_score_pct);
}


static bool best_at_top(move_t* start, move_t* end)
{
    move_t* bestp = 0;
    int32_t bestscore = 0;

    for (move_t* mp=start; mp<end; mp++) {
        if (*mp != 0) {
            int32_t score = get_move_score(*mp);
            if (bestp==0 || score > bestscore) {
                bestp = mp;
                bestscore = score;
            }
        }
    }

    if (bestp) {
        move_t tmp_mv = *start;
        *start = *bestp;
        *bestp = tmp_mv;
        return true;
    }

    return false;
}
