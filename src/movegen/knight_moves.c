#include "movegen_internal.h"

#include "prophet/piece.h"
#include "prophet/position.h"
#include "prophet/square.h"

#include "../bitmap/bitmap.h"
#include "../position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t knight_moves[64];

/**
 * \brief Generate pseudo-legal knight moves
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
move_t* gen_knight_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);
    uint64_t pmap = p->player==WHITE ? p->white_knights : p->black_knights;

    while (pmap) {
        square_t sq = (square_t)get_msb(pmap);
        m = gen_knight_moves_from_sq(m, p, sq, caps, noncaps);
        pmap ^= square_to_bitmap(sq);
    }

    return m;
}

move_t* gen_knight_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps)
{
    assert(m);
    assert(p);
    assert(from >= A8 && from <= H1);

    uint64_t mv_map = get_knight_moves(
        from, get_target_squares(p, caps, noncaps));

    while (mv_map) {
        square_t sq = (square_t)get_lsb(mv_map);
        m = add_move(m, p, KNIGHT, from, sq);
        mv_map ^= square_to_bitmap(sq);
    }

    return m;
}

/**
 * \brief Get knight moves.
 *
 * \param from          the square the knight is moving from
 * \param targets       target squares
 *
 * \return the subset of target squares the knight can move to
 */
uint64_t get_knight_moves(square_t from, uint64_t targets)
{
    return knight_moves[from] & targets;
}

void init_knight_movegen()
{
    for (int i=0; i<64; i++) {
        square_t sq = (square_t)i;
        knight_moves[sq] = 0;

        rank_t r = get_rank(sq);
        file_t f = get_file(sq);

        if (f > FILE_A) {
            if (r > RANK_7) {
                knight_moves[sq] |= square_to_bitmap(north(northwest(sq)));
            }
            if (r < RANK_2) {
                knight_moves[sq] |= square_to_bitmap(south(southwest(sq)));
            }
        }
        if (f > FILE_B) {
            if (r > RANK_8) {
                knight_moves[sq] |= square_to_bitmap(northwest(west(sq)));
            }
            if (r < RANK_1) {
                knight_moves[sq] |= square_to_bitmap(southwest(west(sq)));
            }
        }
        if (f < FILE_G) {
            if (r > RANK_8) {
                knight_moves[sq] |= square_to_bitmap(northeast(east(sq)));
            }
            if (r < RANK_1) {
                knight_moves[sq] |= square_to_bitmap(southeast(east(sq)));
            }
        }
        if (f < FILE_H) {
            if (r > RANK_7) {
                knight_moves[sq] |= square_to_bitmap(north(northeast(sq)));
            }
            if (r < RANK_2) {
                knight_moves[sq] |= square_to_bitmap(south(southeast(sq)));
            }
        }
    }
}
