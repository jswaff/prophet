#include <prophet/position/piece.h>

#include  <assert.h>

#include "eval_internal.h"


/**
 * \brief Determine if a pawn is isolated.
 *
 * An isolated pawn is a pawn that has no friendly pawn on an adjacent file.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is isolated, otherwise false.
 */
bool pawn_isolated(const position_t* pos, square_t pawn_sq)
{
    assert(pos->piece[pawn_sq] == PAWN || pos->piece[pawn_sq] == -PAWN);

    if (is_white_piece(pos->piece[pawn_sq]))
    {

    }
    else /* black pawn */
    {

    }

    return false;
}
