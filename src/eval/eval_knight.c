#include "eval_internal.h"

#include "prophet/position.h"
#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>


/**
 * \brief Evaluate a single knight.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the knight is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_knight(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == KNIGHT || pos->piece[sq] == -KNIGHT);

    if (is_white_piece(pos->piece[sq])) {
        uint32_t d = distance(sq, pos->black_king);
        *mgscore += knight_pst_mg[sq] + knight_tropism_mg * d;
        *egscore += knight_pst_eg[sq] + knight_tropism_eg * d;

        if (outpost(pos, sq, true)) {
            int32_t outpost_score = knight_outpost[sq];
            if (pawn_supports(pos, sq)) {
                outpost_score += knight_supported_outpost[sq];
            }
            *mgscore += outpost_score;
            *egscore += outpost_score;
        }
    } else {
        int32_t flipsq = flip_rank[sq];
        uint32_t d = distance(sq, pos->white_king);
        *mgscore -= knight_pst_mg[flipsq] + knight_tropism_mg * d;
        *egscore -= knight_pst_eg[flipsq] + knight_tropism_eg * d;

        if (outpost(pos, sq, false)) {
            int32_t outpost_score = knight_outpost[flipsq];
            if (pawn_supports(pos, sq)) {
                outpost_score += knight_supported_outpost[flipsq];
            }
            *mgscore -= outpost_score;
            *egscore -= outpost_score;
        }
    }
}
