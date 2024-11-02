#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * \brief Evaluate a single pawn.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the pawn is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_pawn(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == PAWN || pos->piece[sq] == -PAWN);

    if (is_white_piece(pos->piece[sq])) {
        *mgscore += pawn_pst_mg[sq];
        *egscore += pawn_pst_eg[sq];
        if (pawn_passed(pos, sq)) {
            int32_t r = get_rank(sq);
            *mgscore += passed_pawn_mg[r];
            *egscore += passed_pawn_eg[r];
        } if (pawn_doubled(pos, sq)) {
            *mgscore += doubled_pawn_mg;
            *egscore += doubled_pawn_eg;
        } if (pawn_isolated(pos, sq)) {
            *mgscore += isolated_pawn_mg;
            *egscore += isolated_pawn_eg;
        }
    } else {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= pawn_pst_mg[flipsq];
        *egscore -= pawn_pst_eg[flipsq];
        if (pawn_passed(pos, sq)) {
            int32_t flipr = get_rank(flipsq);
            *mgscore -= passed_pawn_mg[flipr];
            *egscore -= passed_pawn_eg[flipr];
        } 
        if (pawn_doubled(pos, sq)) {
            *mgscore -= doubled_pawn_mg;
            *egscore -= doubled_pawn_eg;
        }
        if (pawn_isolated(pos, sq)) {
            *mgscore -= isolated_pawn_mg;
            *egscore -= isolated_pawn_eg;
        }
    }
}
