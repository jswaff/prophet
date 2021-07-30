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
    bool in_check, bool null_move_ok, move_t* move_stack, undo_t* undo_stack, 
    stats_t* stats, search_options_t* opts);

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail);

static void add_killer(move_t killer_move, int ply);

static bool is_draw(const position_t* pos, const undo_t* undo_stack);

static square_t apply_null_move(position_t* pos);
static void undo_null_move(position_t* pos, square_t ep_sq);

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

    bool player_in_check = in_check(pos, pos->player);

    int32_t score = search_helper(pos, parent_pv, true, 0, depth, alpha, beta, 
        player_in_check, false, move_stack, undo_stack, stats, opts);

    /* capture the PV for ordering in the next search */
    memcpy(&last_pv, parent_pv, sizeof(move_line_t));

    return score;
}


static int32_t search_helper(position_t* pos, move_line_t* parent_pv, 
    bool first, int ply, int32_t depth, int32_t alpha, int32_t beta, 
    bool incheck, bool null_move_ok, move_t* move_stack, undo_t* undo_stack, 
    stats_t* stats, search_options_t* opts)
{
    assert(depth >= 0);
    assert(alpha >= -CHECKMATE);
    assert(beta <= CHECKMATE);
    assert(alpha < beta);

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

    uint64_t hash_val = 0, hash_val2 = 0;

    /* try to get an early exit, iff this isn't the root. */
    if (ply > 0) 
    {
        /* draw check */
        if (is_draw(pos, undo_stack))
        {
            stats->draws++;
            return 0;
        }

        /* hash table slot 0 */
        hash_val = probe_hash(&htbl, pos->hash_key, 0);
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

        /* hash table slot 1 */
        /* TODO: if this is helpful, see about cleaning up the redundancy */
        hash_val2 = probe_hash(&htbl, pos->hash_key, 1);
        if (hash_val2 != 0 && get_hash_entry_depth(hash_val2) >= depth) 
        {
            hash_entry_type_t hash_entry_type = get_hash_entry_type(hash_val2);
            if (hash_entry_type == LOWER_BOUND) 
            {
                if (get_hash_entry_score(hash_val2) >= beta) 
                {
                    stats->fail_highs++;
                    stats->hash_fail_highs++;
                    return beta;
                }
            } 
            else if (hash_entry_type == UPPER_BOUND) 
            {
                if (get_hash_entry_score(hash_val2) <= alpha) 
                {
                    stats->fail_lows++;
                    stats->hash_fail_lows++;
                    return alpha;
                }
            } 
            else if (hash_entry_type == EXACT_SCORE) 
            {
                stats->hash_exact_scores++;
                return get_hash_entry_score(hash_val2);
            }
        }

        /* null move */
        if (!first && !incheck && null_move_ok && depth >= 3 && !zugzwang(pos))
        {
            square_t ep_sq = apply_null_move(pos);
            int null_depth = depth - 4; /* R=3 */
            /* don't drop into q-search */
            if (null_depth < 1) { null_depth = 1; } 
            move_line_t mline;
            mline.n = 0;
            int32_t null_score = -search_helper(pos, &mline, false, ply+1, 
                null_depth, -beta, -beta+1, false, false, move_stack, 
                undo_stack, stats, opts);
            undo_null_move(pos, ep_sq);

            if (stop_search) 
            {
                return 0;
            }
            if (null_score >= beta) 
            {
                stats->fail_highs++;
                return beta;
            }
        }
    }

    /* prepare to search */
    int num_moves_searched = 0;
    move_line_t pv; pv.n = 0;

    move_order_dto mo_dto;
    move_t pv_move = first && last_pv.n > ply ? last_pv.mv[ply] : NO_MOVE;
    move_t hash_move = hash_val == 0 ? NO_MOVE : get_hash_entry_move(hash_val);
    move_t hash_move2 = hash_val2 == 0 ? NO_MOVE : get_hash_entry_move(hash_val2);    
    initialize_move_ordering(&mo_dto, move_stack, pv_move, hash_move, hash_move2,
        killer1[ply], killer2[ply], true, true);

    move_t best_move = NO_MOVE;
    move_t* mp;
    undo_t* uptr = undo_stack + pos->move_counter;
    while (next(pos, &mp, &mo_dto))
    {
        assert(get_move_score(*mp)==0);
        apply_move(pos, *mp, uptr);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, uptr);
            continue;
        }

        bool pvnode = first && num_moves_searched==0;
        bool gives_check = in_check(pos, pos->player);

        int ext = gives_check ? 1 : 0;
        bool try_null = ext == 0;

        int32_t score;
        if (num_moves_searched==0 || ply==0)
        {
            score = -search_helper(
                pos, &pv, pvnode, ply+1, depth-1+ext, -beta, -alpha, gives_check, try_null,
                mo_dto.end, undo_stack, stats, opts);
        }
        else
        {
            /* if we've already searched a few moves, chances are this is an ALL node, 
             * and we're not going to get a beta cutoff.  Unless the move looks "interesting"
             * in some way, just search to a reduced depth.  If we're surprised by the 
             * score, we'll research it to the normal depth.
             */
            if (num_moves_searched >= 4 && depth >= 3 && !pvnode && !incheck && 
                !gives_check && ext==0 && !is_capture(*mp) && !get_promopiece(*mp) &&
                *mp != killer1[ply] && *mp != killer2[ply])
            {
                score = -search_helper(
                    pos, &pv, pvnode, ply+1, depth-2, -(alpha+1), -alpha, gives_check, try_null,
                    mo_dto.end, undo_stack, stats, opts);
            }
            else
            {
                score = alpha + 1; /* ensure a search */
            }

            if (score > alpha)
            {
                /* try a PVS (zero width) search */
                score = -search_helper(
                    pos, &pv, pvnode, ply+1, depth-1+ext, -(alpha+1), -alpha, gives_check, try_null,
                    mo_dto.end, undo_stack, stats, opts);

                if (score > alpha && score < beta)
                {
                    score = -search_helper(
                        pos, &pv, pvnode, ply+1, depth-1+ext, -beta, -alpha, gives_check, try_null,
                        mo_dto.end, undo_stack, stats, opts);
                }
            }
        }

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
    assert(alpha > -65536);
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

static square_t apply_null_move(position_t* pos) 
{
    square_t ep_sq = pos->ep_sq;

    /* change player */
    pos->hash_key ^= zkeys.ptm[pos->player];
    pos->player = opposite_player(pos->player);
    pos->hash_key ^= zkeys.ptm[pos->player];

    /* clear EP square */    
    if (pos->ep_sq != NO_SQUARE) 
    {
        pos->hash_key ^= zkeys.ep[pos->ep_sq];
    }
    pos->ep_sq = NO_SQUARE;

    return ep_sq;
}


static void undo_null_move(position_t* pos, square_t ep_sq) 
{
    /* reset EP square */
    pos->ep_sq = ep_sq;
    if (ep_sq != NO_SQUARE)
    {
        pos->hash_key ^= zkeys.ep[pos->ep_sq];
    }

    /* change player */
    pos->hash_key ^= zkeys.ptm[pos->player];
    pos->player = opposite_player(pos->player);
    pos->hash_key ^= zkeys.ptm[pos->player];
}
