#include "eval_internal.h"

#include "position/position.h"

#include <stdbool.h>


bool trapped_bishop(const position_t *pos, square_t sq, bool is_white)
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
