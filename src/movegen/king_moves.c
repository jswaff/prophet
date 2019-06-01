#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <prophet/position/position.h>
#include <prophet/util/bitmap.h>
#include <prophet/parameters.h>

#include "movegen_internal.h"

static uint64_t king_moves[64];

static move* add_castle(move* m, square_t from, square_t to);

/**
 * \brief Generate pseudo-legal king moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_king_moves(move* m, const position* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    square_t sq = p->player==WHITE ? p->white_king : p->black_king;

    return gen_king_moves_from_sq(m, p, sq, caps, noncaps);
}

move* gen_king_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps)
{
    assert(m);
    assert(p);
    assert(from >= A8 && from <= H1);

    uint64_t mv_map = get_king_moves(from, get_target_squares(p, caps, noncaps));

    while (mv_map) {
        square_t sq = (square_t)get_lsb(mv_map);
        m = add_move(m, p, KING, from, sq);
        mv_map ^= square_to_bitmap(sq);
    }

    // castling moves
    if (noncaps) {
        if (p->player==WHITE && from==E1 && !attacked(p, E1, BLACK)) {
            if (can_castle_wq(p) && is_empty_sq(p, D1) && is_empty_sq(p, C1)
                && is_empty_sq(p, B1) && !attacked(p, D1, BLACK))
            {
                m = add_castle(m, E1, C1);
            }
            if (can_castle_wk(p) && is_empty_sq(p, F1) && is_empty_sq(p, G1)
                && !attacked(p, F1, BLACK))
            {
                m = add_castle(m, E1, G1);
            }
        } else if (p->player==BLACK && from==E8 && !attacked(p, E8, WHITE))
        {
            if (can_castle_bq(p) && is_empty_sq(p, D8) && is_empty_sq(p, C8) && is_empty_sq(p, B8)
                && !attacked(p, D8, WHITE))
            {
                m = add_castle(m, E8, C8);
            }
            if (can_castle_bk(p) && is_empty_sq(p, F8) && is_empty_sq(p, G8)
                && !attacked(p, F8, WHITE))
            {
                m = add_castle(m, E8, G8);
            }
        }
    }

    return m;
}

/**
 * \brief Get king moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the king is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the king can move to.
 */
uint64_t get_king_moves(square_t from, uint64_t targets)
{
    return king_moves[from] & targets;
}

static move* add_castle(move* m, square_t from, square_t to)
{
    *m = to_move(KING, from, to);
    set_castle(m);

    ++m;

    return m;
}

void init_king_movegen()
{
    for (int i=0; i<64; i++) {
        square_t sq = (square_t)i;
        king_moves[sq] = 0;

        rank_t r = get_rank(sq);
        file_t f = get_file(sq);

        if (r > RANK_8) {
            if (f > FILE_A) {
                king_moves[sq] |= square_to_bitmap(northwest(sq));
            }
            king_moves[sq] |= square_to_bitmap(north(sq));
            if (f < FILE_H) {
                king_moves[sq] |= square_to_bitmap(northeast(sq));
            }
        }
        if (f < FILE_H) {
            king_moves[sq] |= square_to_bitmap(east(sq));
        }
        if (r < RANK_1) {
            if (f < FILE_H) {
                king_moves[sq] |= square_to_bitmap(southeast(sq));
            }
            king_moves[sq] |= square_to_bitmap(south(sq));
            if (f > FILE_A) {
                king_moves[sq] |= square_to_bitmap(southwest(sq));
            }
        }
        if (f > FILE_A) {
            king_moves[sq] |= square_to_bitmap(west(sq));
        }
    }
}

