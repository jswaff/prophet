#include <assert.h>
#include <stdint.h>

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

/**
 * \brief Get the set of target squares
 *
 * If \p caps is true, the targets include the opposite player's pieces.  If 
 * \p noncaps is set, targets includes all empty squares.  Note the two 
 * options are not mutually exclusive.
 *
 * \param p             A pointer to a chess position
 * \param caps          If capture targets should be included
 * \param noncaps       If non-capture targets should be included
 *
 * \return The set of target squares
 */
uint64_t get_target_squares(const position* p, bool caps, bool noncaps)
{
    uint64_t targets = 0;

    if (caps) {
        targets = p->player == WHITE ? p->black_pieces : p->white_pieces;
    }

    if (noncaps) {
        targets |= ~(p->white_pieces | p->black_pieces);
    }

    return targets;
}

/**
 * \brief Generate a moves mask in one direction based on the set of occupied 
 * squares
 *
 * From the square \p sq, add all squares in one direction until the edge of 
 * the board or until an occupied square is encountered.
 *
 * \param sq            The square to start from
 * \param occupied      The set of occupied squares
 * \param dir_func      A function giving the next square for a given direction
 *
 * \return  The set of squares that can be moved to.
 */
uint64_t gen_moves_mask(square_t sq, uint64_t occupied, dir_func_t dir_func)
{
    uint64_t mask = 0;

    square_t to = dir_func(sq);

    while (to != NO_SQUARE) 
    {
        uint64_t bb_to = square_to_bitmap(to);
        mask |= bb_to;
        if (bb_to & occupied) 
        {
            break;
        }
        to = dir_func(to);
    }

    return mask;
}

/**
 * \brief Count the number of capture and non-capture moves
 *
 * The memory range is iterated, beginning with \p startp and ending with 
 * \p endp - 1. Some slots may contain an invalid move (BADMOVE).  These 
 * "moves" are not counted.
 *
 * \param startp        The starting address of a list of moves (inclusive)
 * \param endp          The ending address of a list of moves (exclusive)
 * \param caps          A pointer to an integer to receive the number of 
 *                      captures
 * \param noncaps       A pointer to an integer to receive the number of 
 *                      noncaptures
 */
void num_moves(move* startp, move* endp, int* caps, int* noncaps)
{
    *caps = 0; *noncaps = 0;

    for (move* mp=startp; mp<endp; mp++) 
    {
        if (*mp != 0) 
        {
            if (is_capture(*mp) || get_promopiece(*mp)) 
            {
                (*caps)++;
            } 
            else 
            {
                (*noncaps)++;
            }
        }
    }
}

/**
 * \brief Given position \p pos, is \p player in check?
 *
 * \param pos       The chess position
 * \param player    A player (white or black)
 *
 * \return - true if the player is in check, otherwise false.
 */
bool in_check(const position* pos, color_t player)
{
    square_t king_sq = player==WHITE ? pos->white_king : pos->black_king;

    return attacked(pos, king_sq, opposite_player(player));
}

