#include <prophet/const.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/util/output.h>
#include <prophet/util/p4time.h>
#include <prophet/util/string_utils.h>

#include "search_internal.h"

#include <stdlib.h>

extern int32_t max_depth;
extern bool xboard_post_mode;

static void print_search_summary(int32_t last_depth, int32_t start_time, 
    const stats_t* stats);
static bool best_at_top(move_t* start, move_t* end);

move_line_t iterate(position_t* pos, bool test_suite_mode, move_t* move_stack, 
    undo_t* undo_stack)
{

    move_line_t pv; pv.n = 0;

    /* TODO: book probe */

    /* if just one legal move, don't bother searching */
    if (!test_suite_mode)
    {
        move_t* endp = gen_legal_moves(move_stack, pos, true, true);
        int num_caps, num_noncaps;
        num_moves_in_list(move_stack, endp, &num_caps, &num_noncaps);
        if (num_caps + num_noncaps == 1)
        {
            best_at_top(move_stack, endp);
            pv.mv[0] = move_stack[0];
            pv.n = 1;
            return pv;
        }
    }

    /* prepare to search */
    int32_t start_time = milli_timer();
    int32_t depth = 0;
    bool stop_searching = false;
    int32_t score = 0;
    stats_t stats;

    /* search using iterative deepening */
    do {
        ++depth;

        int32_t alpha_bound = -INF;
        int32_t beta_bound = INF;

        score = search(pos, &pv, depth, alpha_bound, beta_bound, move_stack, 
            undo_stack, &stats);

        /* print the move line */
        if (xboard_post_mode)
        {
            char* pv_buf = move_line_to_str(&pv);
            int32_t time_centis = (milli_timer() - start_time) / 10;
            out(stdout, "%2d %5d %5d %7llu %s\n", depth, score, time_centis,
                stats.nodes, pv_buf);
            free(pv_buf);
        }

        /* if the search discovered a checkmate, stop */
        if (abs(score) > CHECKMATE - 500)
        {
            stop_searching = true;
        }

        /* if the search has reached the maximum depth, stop */
        if (max_depth > 0 && depth >= max_depth)
        {
            stop_searching = true;
        }


    } while (!stop_searching);

    /* TODO: assert the PV is valid */
    assert(pv.n > 0);

    /* print the search summary */
    print_search_summary(depth, start_time, &stats);

    return pv;
}


static void print_search_summary(int32_t last_depth, int32_t start_time, 
    const stats_t* stats)
{

    out(stdout, "\n");
    out(stdout, "# depth: %d\n", last_depth);

    /* print node counts */
    uint64_t total_nodes = stats->nodes;

    float interior_pct = stats->nodes / (total_nodes/100.0);
    float qnode_pct = 0.0; // stats->qnodes / (total_nodes/100.0);    

    out(stdout, 
        "# nodes: %lluk, interior: %lluk (%.2f%%), quiescence: %lluk (%.2f%%)\n",
        total_nodes/1000, stats->nodes/1000, interior_pct, 0.0, qnode_pct);

    /* calculate the search time.  add 1 ms just to avoid div by 0 */
    int32_t search_time_ms = milli_timer() - start_time + 1; 
    float search_time = search_time_ms / 1000.0;
    uint64_t nps = total_nodes / search_time_ms;
    out(stdout, "# search time: %.2f seconds, rate: %llu kn/s\n",
        search_time,nps);
}

/* TODO: this is a duplicate of the function in next */
static bool best_at_top(move_t* start, move_t* end)
{
    move_t* bestp = 0;
    int32_t bestscore = 0;

    for (move_t* mp=start; mp<end; mp++)
    {
        if (*mp != 0)
        {
            int32_t score = get_move_score(*mp);
            if (bestp==0 || score > bestscore)
            {
                bestp = mp;
                bestscore = score;
            }
        }
    }

    if (bestp)
    {
        move_t tmp_mv = *start;
        *start = *bestp;
        *bestp = tmp_mv;
        return true;
    }

    return false;
}
