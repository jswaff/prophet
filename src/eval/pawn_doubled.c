#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/position.h"

#include "../bitmap/bitmap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


/**
 * \brief Determine if a pawn is doubled.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is doubled, otherwise false.
 */
bool pawn_doubled(const position_t* pos, square_t pawn_sq)
{
    assert(pos->piece[pawn_sq] == PAWN || pos->piece[pawn_sq] == -PAWN);

    uint64_t file_mask = 
        sq_to_file_bitmap(pawn_sq) ^ square_to_bitmap(pawn_sq);

    if (is_white_piece(pos->piece[pawn_sq])) {
        return file_mask & pos->white_pawns;
    } 
    else /* black pawn */
    {
        return file_mask & pos->black_pawns;
    }
}
