#include "eval_internal.h"

#include "prophet/position/piece.h"
#include "prophet/position/position.h"
#include "prophet/position/square.h"

#include "../bitmap/bitmap.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


static uint64_t bb_isolated[64];

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

    if (is_white_piece(pos->piece[pawn_sq])) {
        return !(bb_isolated[pawn_sq] & pos->white_pawns);
    }
    else /* black pawn */
    {
        return !(bb_isolated[pawn_sq] & pos->black_pawns);
    }
}

/**
 * \brief Initialize the pawn_isolated function.
 */
void init_pawn_isolated()
{
    for (int i=0; i<64; i++) {
        bb_isolated[i] = 0;
        file_t f = get_file((square_t)i);

        if (f > FILE_A) {
            bb_isolated[i] |= file_to_bitmap(f-1);
        }
        if (f < FILE_H)	{
            bb_isolated[i] |= file_to_bitmap(f+1);
        }
    }
}
