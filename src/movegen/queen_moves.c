#include "movegen_internal.h"

#include "prophet/position.h"
#include "prophet/square.h"

#include "../bitmap/bitmap.h"
#include "../position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Generate pseudo-legal queen moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_queen_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);
    uint64_t pmap = p->player==WHITE ? p->white_queens : p->black_queens;

    while (pmap) {
        square_t sq = (square_t)get_msb(pmap);
        m = gen_queen_moves_from_sq(m, p, sq, caps, noncaps);
        pmap ^= square_to_bitmap(sq);
    }

    return m;
}

move_t* gen_queen_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps)
{
    assert(m);
    assert(p);
    assert(from >= A8 && from <= H1);

    uint64_t queen_moves = get_queen_moves(
        p, from, get_target_squares(p, caps, noncaps));

    while (queen_moves) {
        square_t sq = (square_t)get_msb(queen_moves);
        m = add_move(m, p, QUEEN, from, sq);
        queen_moves ^= square_to_bitmap(sq);
    }

    return m;
}

/**
 * \brief Get queen moves.
 *
 * \param p             a pointer to a chess position
 * \param from          the square the queen is moving from
 * \param targets       target squares
 *
 * \return the subset of target squares the queen can move to
 */
uint64_t get_queen_moves(const position_t* p, square_t from, uint64_t targets)
{
    return get_rook_moves(p, from, targets) | 
           get_bishop_moves(p, from, targets);
}
