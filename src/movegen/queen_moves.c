#include "movegen_internal.h"

#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>


move_t* gen_queen_moves(move_t *m, const position_t *p, bool caps, bool noncaps)
{
    assert(caps || noncaps);
    uint64_t pmap = p->player==WHITE ? p->white_queens : p->black_queens;

    while (pmap) {
        square_t sq = (square_t)get_lsb(pmap);
        m = gen_queen_moves_from_sq(m, p, sq, caps, noncaps);
        pmap ^= square_to_bitmap(sq);
    }

    return m;
}


move_t* gen_queen_moves_from_sq(move_t *m, const position_t *p, square_t from, bool caps, bool noncaps)
{
    assert(m);
    assert(p);
    assert(from >= A8 && from <= H1);

    uint64_t queen_moves = get_queen_moves(p, from) & get_target_squares(p, caps, noncaps);

    while (queen_moves) {
        square_t sq = (square_t)get_lsb(queen_moves);
        m = add_move(m, p, QUEEN, from, sq);
        queen_moves ^= square_to_bitmap(sq);
    }

    return m;
}


uint64_t get_queen_moves(const position_t *p, square_t from)
{
    return get_rook_moves(p, from) | get_bishop_moves(p, from);
}
