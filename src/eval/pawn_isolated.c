#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


static uint64_t bb_isolated[64];


bool pawn_isolated(const position_t *pos, square_t pawn_sq)
{
    assert(pos->piece[pawn_sq] == PAWN || pos->piece[pawn_sq] == -PAWN);

    if (is_white_piece(pos->piece[pawn_sq])) {
        return !(bb_isolated[pawn_sq] & pos->white_pawns);
    } else { /* black pawn */
        return !(bb_isolated[pawn_sq] & pos->black_pawns);
    }
}


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
