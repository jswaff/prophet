#include "movegen_internal.h"

#include "prophet/move.h"
#include "prophet/piece.h"

#include "position/position_internal.h"

#include <assert.h>

move_t* add_move(move_t *m, const position_t *p, piece_t piece, square_t from, square_t to)
{
    piece_t captured_piece = (piece_t)p->piece[to];

    assert( (p->player==WHITE && is_not_white_piece(captured_piece))
            || (p->player==BLACK && is_not_black_piece(captured_piece)));

    *m = to_capture(piece, from, to, captured_piece);

    ++m;

    return m;
}
