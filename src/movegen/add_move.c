#include "movegen_internal.h"

#include "prophet/move.h"
#include "prophet/piece.h"
#include "prophet/position.h"

#include <assert.h>

/**
 * \brief Add a move to a move list
 *
 * Creates a move and adds it to a move list.  This method should not be used
 * for pawn promotions, en-passant captures, or for castling moves.
 *
 * \param m             a pointer to a move to set
 * \param p             a pointer to a chess position
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 *
 * \return the next move pointer
 */
move_t* add_move(move_t* m, const position_t* p, piece_t piece, square_t from, square_t to)
{
    piece_t captured_piece = (piece_t)p->piece[to];

    assert( (p->player==WHITE && is_not_white_piece(captured_piece))
            || (p->player==BLACK && is_not_black_piece(captured_piece)));

    *m = to_capture(piece, from, to, captured_piece);

    ++m;

    return m;
}
