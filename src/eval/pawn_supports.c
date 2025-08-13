#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "movegen/movegen_internal.h"

#include <stdbool.h>


/**
 * \brief Determine if a pawn supports a sq.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square in question
 *
 * \return true if a friendly pawn supports, otherwise false.
 */ 
bool pawn_supports(const position_t* pos, square_t sq)
{
    if (is_white_piece(pos->piece[sq])) {
        return (get_pawn_attacks(sq, BLACK) & pos->white_pawns) > 0;
    }
    else {
        return (get_pawn_attacks(sq, WHITE) & pos->black_pawns) > 0;
    }
}
