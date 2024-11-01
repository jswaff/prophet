#include "search_internal.h"

#include "prophet/movegen.h"
#include "prophet/position.h"

#include "../movegen/movegen_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

static move_t* index_best_capture(move_t* start, move_t* end);
static void swap_moves(move_t* mv1, move_t* mv2);
static move_t* next_nonnull_move(move_t* start, move_t* end);

/**
 * \brief Determine the next move to play.
 *
 * \param pos           the chess position 
 * \param m             pointer to a move pointer to be set to the selected
 *                      move
 * \param mo            the move ordering context
 *
 * \return true if a move has been selected, or false if there are no
 *    further moves.
 */
bool next(const position_t* pos, move_t** m, move_order_dto* mo)
{
    if (mo->next_stage == PV) {
        mo->next_stage = HASH_MOVE;

        if (mo->pv_move != NO_MOVE) {
            assert(is_legal_move(mo->pv_move, pos));
            *m = &mo->pv_move;
            return true;
        }
    }

    if (mo->next_stage == HASH_MOVE) {
        mo->next_stage = GEN_CAPS;

        if (mo->hash_move != mo->pv_move && good_move(pos, mo->hash_move)) {
            assert(is_legal_move(mo->hash_move, pos));
            *m = &mo->hash_move;
            return true;
        }
    }

    if (mo->next_stage == GEN_CAPS) {
        mo->next_stage = GOOD_CAPTURES_PROMOS;

        assert(mo->start);

        /* generate the captures */
        mo->end = gen_pseudo_legal_moves(mo->start, pos, true, false);
        mo->current = mo->start;

        /* score the captures, and remove any special moves already played */
        for (move_t* mp=mo->start; mp<mo->end; mp++) {
            if (*mp==mo->pv_move || *mp==mo->hash_move) {
                *mp = 0;
            } else {
                set_move_score(mp, mvvlva(*mp));
            }
        }
    }

    if (mo->next_stage == GOOD_CAPTURES_PROMOS) {
        move_t* bestcap = index_best_capture(mo->current, mo->end);
        while (bestcap) {
            assert(*bestcap);
            assert(get_piece(*bestcap) != NO_PIECE);
            assert(get_promopiece(*bestcap) != NO_PIECE || get_captured_piece(*bestcap) != NO_PIECE);

            /* the best by MVV/LVA doesn't mean the move is good.  it's good if:
             * 1. it's a promotion
             * 2. the value of the captured piece >= value of capturing piece
             * 3. SEE analysis gives a non-negative score
             * only do SEE if necessary, but if we do, keep the score for sorting bad
             * captures later on.
             */ 
            bool good_cap = get_promopiece(*bestcap) != NO_PIECE || 
                 (see_eval_piece(get_captured_piece(*bestcap)) >= see_eval_piece(get_piece(*bestcap)));
            int32_t see_score = -INF;    
            if (!good_cap) {
                see_score = see(pos, *bestcap);
                good_cap = see_score >= 0;
            }

            /* put the selected move at the top of the list */
            swap_moves(bestcap, mo->current);

            /* if the capture is good, play it now.  Otherwise, defer it until later and move
             * onto the next item.
             */
            if (good_cap) {
                set_move_score(mo->current, 0); /* signal that this move has been played */
                *m = mo->current;
                mo->current++;
                return true;
            } else {
                /* defer bad capture for later */
                assert(see_score < 0);
                set_move_score(mo->current, see_score < -CHECKMATE ? -CHECKMATE : see_score);
                mo->current++;
                bestcap = index_best_capture(mo->current, mo->end);
            }
        }
        mo->next_stage = KILLER1;
    }

    if (mo->next_stage == KILLER1) {
        mo->next_stage = KILLER2;
        if (mo->killer1 != mo->pv_move && mo->killer1 != mo->hash_move &&
            good_move(pos, mo->killer1))
        {
            assert(!is_capture(mo->killer1));
            *m = &mo->killer1;
            return true;
        }
    }

    if (mo->next_stage == KILLER2) {
        mo->next_stage = mo->gen_noncaps? GEN_NONCAPS : INIT_BAD_CAPTURES;
        if (mo->killer2 != mo->pv_move && mo->killer2 != mo->hash_move && 
            mo->killer2 != mo->killer1 && good_move(pos, mo->killer2))
        {
            assert(!is_capture(mo->killer2));
            *m = &mo->killer2;
            return true;
        }
    }

    if (mo->gen_noncaps) {
        if (mo->next_stage == GEN_NONCAPS) {
            mo->next_stage = NONCAPS;
            mo->current = mo->end;
            mo->end = gen_pseudo_legal_moves(mo->current, pos, false, true);
            /* remove any moves already tried */
            for (move_t* mp=mo->current; mp<mo->end; mp++) {
                if (*mp==mo->pv_move || *mp==mo->hash_move || 
                    *mp==mo->killer1 || *mp==mo->killer2)
                {
                    *mp = 0;
                }
            }
        }

        if (mo->next_stage == NONCAPS) {
            /* play noncaptures as they come */
            while (mo->current < mo->end) {
                *m = mo->current;
                mo->current++;
                if (**m != 0) {
                    return true;
                }
            }
            mo->next_stage = INIT_BAD_CAPTURES;
        }
    }

    if (mo->play_badcaps) {
        if (mo->next_stage == INIT_BAD_CAPTURES) {
            for (move_t* mp=mo->start; mp<mo->end; mp++) {
                /* remove everything except captures with losing scores */
                bool losing_capture = is_capture(*mp) && get_move_score(*mp) < 0;
                if (!losing_capture) {
                    *mp = 0;
                }
            }
            mo->current = mo->start;
            mo->next_stage = BAD_CAPTURES;
        }

        /* now just go back through the list playing the best option we have 
         * Implementation note: advancing the pointer to the next non-null move is not
         * necessary.  It is only here to match the method used in chess4j, so the
         * search trees are equal. */
        move_t* nextp = next_nonnull_move(mo->current, mo->end);
        if (nextp) {
            mo->current = nextp;
            move_t* best_bad_cap = index_best_capture(mo->current, mo->end);
            if (best_bad_cap) {
                assert(is_capture(*best_bad_cap));
                assert(get_promopiece(*best_bad_cap)==NO_PIECE);
                swap_moves(best_bad_cap, mo->current);
                set_move_score(mo->current, 0);
                *m = mo->current;
                mo->current++;
                return true;
            }
        }
    }

    return false;
}


static move_t* index_best_capture(move_t* start, move_t* end)
{
    move_t* bestp = 0;
    int32_t bestscore = -INF;

    for (move_t* mp=start; mp<end; mp++) {
        if (*mp != 0) {
            assert(is_capture(*mp) || get_promopiece(*mp) != NO_PIECE);
            int32_t score = get_move_score(*mp);
            if (score > bestscore) {
                bestp = mp;
                bestscore = score;
            }
        }
    }

    return bestp;
}

static void swap_moves(move_t* mv1, move_t* mv2)
{
    move_t tmp_mv = *mv2;
    *mv2 = *mv1;
    *mv1 = tmp_mv;
}

static move_t* next_nonnull_move(move_t* start, move_t* end)
{
    for (move_t* mp=start;mp<end; mp++) {
        if (*mp != 0) {
            return mp;
        }
    }
    return 0;
}
