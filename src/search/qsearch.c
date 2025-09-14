#include "prophet/search.h"

#include "search_internal.h"
#include "eval/eval_internal.h"
#include "movegen/movegen_internal.h"
#include "nn/nn_internal.h"
#include "position/position.h"
#include "util/time.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

extern bool volatile stop_search;
extern bool volatile skip_time_checks;

extern neural_network_t neural_network;
extern bool use_neural_network;


int32_t qsearch(position_t *pos, int32_t alpha, int32_t beta, move_t *move_stack, undo_t *undo_stack, stats_t *stats,
    search_options_t *opts)
{
    assert(alpha < beta);

    /* time check */
    if (!skip_time_checks && stop_search_on_time(opts, stats)) {
        stop_search = true;
        return 0;
    }

    stats->qnodes++;

    int32_t stand_pat = use_neural_network ? nn_eval(pos, &neural_network) : eval(pos, false, true);
    if (stand_pat > alpha) {
        if (stand_pat >= beta) {
            return stand_pat;
        }
        /* the static evaulation is our lower bound */
        alpha = stand_pat;
    }

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, move_stack, NO_MOVE, NO_MOVE, NO_MOVE, NO_MOVE, false, false);

    move_t *mp;
    undo_t *uptr = undo_stack + pos->move_counter;
    while (next(pos, &mp, &mo_dto)) {
        assert(get_move_score(*mp)==0);

        /* if this is a capture, can it possibly raise alpha? (delta pruning) */
        if (is_capture(*mp) && get_promopiece(*mp)==NO_PIECE && 
            stand_pat + see_eval_piece(get_captured_piece(*mp)) + see_pawn_val * 2 < alpha)
        {
            continue;
        }

        apply_move(pos, *mp, uptr);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player))) {
            undo_move(pos, uptr);
            continue;
        }

        int32_t score = -qsearch(pos, -beta, -alpha, mo_dto.end, undo_stack, stats, opts);

        undo_move(pos, uptr);

        /* if the search was stopped we can't trust these results */
        if (stop_search) {
            return 0;
        }

        if (score >= beta) {
            return score;
        }
        if (score > alpha) {
            alpha = score;
        }
    }

    return alpha;
}
