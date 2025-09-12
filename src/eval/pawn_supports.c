#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/square.h"

#include "movegen/movegen_internal.h"
#include "position/position.h"

#include <stdbool.h>


bool pawn_supports(const position_t *pos, square_t sq)
{
    if (is_white_piece(pos->piece[sq])) {
        return (get_pawn_attacks(sq, BLACK) & pos->white_pawns) > 0;
    }
    else {
        return (get_pawn_attacks(sq, WHITE) & pos->black_pawns) > 0;
    }
}
