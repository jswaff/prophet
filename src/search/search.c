#include <prophet/const.h>
#include <prophet/hash.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/util/p4time.h>

#include "search_internal.h"

#include <assert.h>
#include <string.h>

move_line_t last_pv;
bool volatile stop_search;
bool volatile skip_time_checks = false;

extern hash_table_t htbl;
extern move_t killer1[MAX_PLY];
extern move_t killer2[MAX_PLY];

static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply);

static int32_t search_helper(position_t* pos, move_line_t* parent_pv, 
    bool first, int ply, int32_t depth, int32_t alpha, int32_t beta, 
    move_t* move_stack, undo_t* undo_stack, stats_t* stats, 
    search_options_t* opts);

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail);

static void add_killer(move_t killer_move, int ply);

static bool is_draw(const position_t* pos, const undo_t* undo_stack);


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param parent_pv     a pointer to the move line that will receive the PV
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param undo_stack    pre-allocated stack for undo information
 * \param stats         structure for tracking search stats
 * \param opts          structure for tracking search options data
 * 
 * \return the score
 */
int32_t search(position_t* pos, move_line_t* parent_pv, int32_t depth, 
    int32_t alpha, int32_t beta, move_t* move_stack, undo_t* undo_stack, 
    stats_t* stats, search_options_t* opts)
{
    assert(move_stack);
    assert(undo_stack);
    
    /* initialize the move line structure */
    memset(parent_pv, 0, sizeof(move_line_t));

    /* initialize the killer move arrays */
    memset(killer1, 0, sizeof(move_t) * MAX_PLY);
    memset(killer2, 0, sizeof(move_t) * MAX_PLY);

    int32_t score = search_helper(pos, parent_pv, true, 0, depth, alpha, beta, 
        move_stack, undo_stack, stats, opts);

    /* capture the PV for ordering in the next search */
    memcpy(&last_pv, parent_pv, sizeof(move_line_t));

    return score;
}


static int32_t search_helper(position_t* pos, move_line_t* parent_pv, 
    bool first, int ply, int32_t depth, int32_t alpha, int32_t beta, 
    move_t* move_stack, undo_t* undo_stack, stats_t* stats, 
    search_options_t* opts)
{
    assert (depth >= 0);

    parent_pv->n = 0;

    if (ply >= MAX_PLY)
    {
        return beta;
    }

    /* time check */
    if (!skip_time_checks && stop_search_on_time(opts, stats))
    {
        stop_search = true;
        return 0;
    }

    /* base case */
    if (depth == 0)
    {
        return qsearch(pos, alpha, beta, move_stack, undo_stack, stats, opts);
    }

    /* this is an interior node */
    stats->nodes++;

    /* try to get an early exit, iff this isn't the root. */
    if (ply > 0) 
    {
        /* draw check */
        if (is_draw(pos, undo_stack))
        {
            stats->draws++;
            return 0;
        }

        /* check the hash table */
        uint64_t hash_val = probe_hash(&htbl, pos->hash_key);
        if (hash_val != 0 && get_hash_entry_depth(hash_val) >= depth) 
        {
            hash_entry_type_t hash_entry_type = get_hash_entry_type(hash_val);
            if (hash_entry_type == LOWER_BOUND) 
            {
                if (get_hash_entry_score(hash_val) >= beta) 
                {
                    stats->fail_highs++;
                    stats->hash_fail_highs++;
                    return beta;
                }
            } 
            else if (hash_entry_type == UPPER_BOUND) 
            {
                if (get_hash_entry_score(hash_val) <= alpha) 
                {
                    stats->fail_lows++;
                    stats->hash_fail_lows++;
                    return alpha;
                }
            } 
            else if (hash_entry_type == EXACT_SCORE) 
            {
                stats->hash_exact_scores++;
                return get_hash_entry_score(hash_val);
            }
        }
    }

    /* prepare to search */
    int num_moves_searched = 0;
    move_line_t pv; pv.n = 0;

    move_order_dto mo_dto;
    move_t pv_move = first && last_pv.n > ply ? last_pv.mv[ply] : NO_MOVE;
    initialize_move_ordering(&mo_dto, move_stack, pv_move, killer1[ply], 
        killer2[ply], true);

    move_t best_move = NO_MOVE;
    move_t* mp;
    undo_t* uptr = undo_stack + pos->move_counter;
    while (next(pos, &mp, &mo_dto))
    {
        apply_move(pos, *mp, uptr);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, uptr);
            continue;
        }

        bool pvnode = first && num_moves_searched==0;
        int32_t score = -search_helper(
            pos, &pv, pvnode, ply+1, depth-1, -beta, -alpha, mo_dto.end, 
            undo_stack, stats, opts);
        ++num_moves_searched;

        undo_move(pos, uptr);

        /* if the search was stopped we can't trust these results */
        if (stop_search)
        {
            return 0;
        }

        /* if this move is "too good", stop here */
        if (score >= beta)
        {
            stats->fail_highs++;
            store_hash_entry(&htbl, pos->hash_key, 
                build_hash_val(LOWER_BOUND, depth, beta, *mp));
            if (!is_capture(*mp) && !get_promopiece(*mp))
            {
                add_killer(*mp, ply);
            }
            return beta;
        }

        /* if this move improves alpha, we should back it up */
        if (score > alpha)
        {
            alpha = score;
            best_move = *mp;
            set_parent_pv(parent_pv, *mp, &pv);
            if (ply == 0 && opts->pv_callback)
            {
                uint64_t elapsed = milli_timer() - opts->start_time;
                opts->pv_callback(parent_pv, depth, score, elapsed, 
                    stats->nodes);
            }
        }
    }

    alpha = adjust_score_for_mate(pos, alpha, num_moves_searched, ply);

    /* since we're here, we didn't get a cutoff.  store the result in the 
     * hash table 
     */
    hash_entry_type_t tet;
    if (best_move == NO_MOVE)
    {
        tet = UPPER_BOUND;
        stats->fail_lows++;
    }
    else
    {
        tet = EXACT_SCORE;
    }
    store_hash_entry(&htbl, pos->hash_key, 
        build_hash_val(tet, depth, alpha, best_move));

    return alpha;
}


static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply)
{
    int32_t adjusted_score = score;

    if (num_moves_searched == 0)
    {
        if (in_check(pos, pos->player))
        {
            adjusted_score = -CHECKMATE + ply;
        }
        else
        {
            /* draw score */
            adjusted_score = 0;
        }
    }

    return adjusted_score;
}

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail)
{
    assert(head != NO_MOVE);

    /* set the head of the PV, without any scoring fields */
    parent_pv->mv[0] = clear_score(head);

    /* set the tail of the PV */
    memcpy(parent_pv->mv + 1, tail->mv, tail->n * sizeof(move_t));

    /* set the size of the line */
    parent_pv->n = tail->n + 1;
}

static void add_killer(move_t killer_move, int ply)
{
    if (killer_move != killer1[ply])
    {
        if (killer_move != killer2[ply]) 
        {
            /* shift */
            killer2[ply] = killer1[ply];
            killer1[ply] = killer_move;
        }
        else
        {
            /* swap them */
            move_t tmp = killer1[ply];
            killer1[ply] = killer2[ply];
            killer2[ply] = tmp;
        }
    }
}

static bool is_draw(const position_t* pos, const undo_t* undo_stack)
{
    return pos->fifty_counter >= 100 
        || is_lack_of_mating_material(pos)
        || is_draw_rep(pos, undo_stack);
}
