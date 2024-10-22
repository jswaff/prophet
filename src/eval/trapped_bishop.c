#include <prophet/bitmap.h>
#include <prophet/position/piece.h>

#include  <assert.h>

#include "eval_internal.h"


/**
 * \brief Determine if bishop on square would be trapped.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the square
 * \param is_white      whether to examine from white's POV
 *
 * \return true if the bishop would be trapped, otherwise false.
 */
bool trapped_bishop(const position_t* pos, square_t sq, bool is_white)
{
    if (is_white) {
        if (sq == A7) {
            return pos->piece[B6]==-PAWN && pos->piece[C7]==-PAWN;
        } else if (sq == H7) {
           return pos->piece[F7]==-PAWN && pos->piece[G6]==-PAWN;	
        }
    } else {
        if (sq == A2) {
            return pos->piece[B3]==PAWN && pos->piece[C2]==PAWN;
        } else if (sq == H2) {
            return pos->piece[F2]==PAWN && pos->piece[G3]==PAWN;	
        }
    }

    return false;
}
