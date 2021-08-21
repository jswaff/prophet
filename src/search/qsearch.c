#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/util/p4time.h>

#include "search_internal.h"

#include <assert.h>


extern bool volatile stop_search;
extern bool volatile skip_time_checks;


/**
 * \brief Search the position to until is is "quiet".
 *
 * Quiescence search - attempt to obtain a score by searching until the 
 * position is quiet.
 *
 * \param pos           a pointer to a chess position
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param undo_stack    pre-allocated stack for undo information
 * \param stats         structure for tracking search stats
 * \param opts          structure for tracking search options data
 * 
 * \return the score
 */
int32_t qsearch(position_t* pos, int32_t alpha, int32_t beta, 
    move_t* move_stack, undo_t* undo_stack, stats_t* stats, 
    search_options_t* opts)
{

    assert(alpha < beta);

    /* time check */
    if (!skip_time_checks && stop_search_on_time(opts, stats))
    {
        stop_search = true;
        return 0;
    }

    stats->qnodes++;

    int32_t stand_pat = eval(pos, false);
    if (stand_pat > alpha) 
    {
        if (stand_pat >= beta) 
        {
            return beta;
        }
        /* the static evaulation is our lower bound */
        alpha = stand_pat;
    }

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, move_stack, NO_MOVE, NO_MOVE, 
        NO_MOVE, NO_MOVE, false, false);

    move_t* mp;
    undo_t* uptr = undo_stack + pos->move_counter;
    while (next(pos, &mp, &mo_dto))
    {
        assert(get_move_score(*mp)==0);

        /* if this is a capture, can it possibly raise alpha? (delta pruning) */
        if (is_capture(*mp) && get_promopiece(*mp)==NO_PIECE && 
            stand_pat + eval_piece(get_captured_piece(*mp)) + pawn_val * 2 < alpha)
        {
            continue;
        }

        apply_move(pos, *mp, uptr);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, uptr);
            continue;
        }

        int32_t score = -qsearch(
            pos, -beta, -alpha, mo_dto.end, undo_stack, stats, opts);

        undo_move(pos, uptr);

        /* if the search was stopped we can't trust these results */
        if (stop_search)
        {
            return 0;
        }

        if (score >= beta)
        {
            return beta;
        }
        if (score > alpha)
        {
            alpha = score;
        }
    }


    return alpha;
}