#include <assert.h>
#include <stdint.h>

#include <prophet/const.h>
#include <prophet/parameters.h>

#include "movegen_internal.h"

/**
 * \brief Add a non-capturing move to a move list
 *
 * Creates a non-capturing move and adds it to a move list.
 *
 * \param m         A pointer to a move to set
 * \param p         A pointer to a chess position
 * \param piece     The moving piece
 * \param from      The square the piece is moving from
 * \param to        The square the piece is moving to
 *
 * \return  The next move pointer
 */
move* add_move(
    move* m, const position* p, piece_t piece, square_t from, square_t to)
{
    piece_t captured_piece = (piece_t)p->piece[to];

    assert( (p->player==WHITE && is_not_white_piece(captured_piece))
            || (p->player==BLACK && is_not_black_piece(captured_piece)));

    *m = to_move(piece, from, to);
    if (captured_piece != NO_PIECE) 
    {
        set_capture(m, captured_piece);
    }
    ++m;

    return m;
}
