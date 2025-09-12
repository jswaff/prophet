#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "movegen/movegen_internal.h"
#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * @brief Evaluate a single queen.
 *
 * @param pos           a pointer to a chess position
 * @param sq            the square the queen is on
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_queen(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == QUEEN || pos->piece[sq] == -QUEEN);

    uint64_t empty_squares = ~(pos->white_pieces | pos->black_pieces);
    uint32_t mobility = popcnt(get_queen_moves(pos, sq) & empty_squares);
    uint32_t mobility_mg = queen_mobility_mg[mobility];
    uint32_t mobility_eg = queen_mobility_eg[mobility];


    if (is_white_piece(pos->piece[sq])) {
        *mgscore += queen_pst_mg[sq] + mobility_mg;
        *egscore += queen_pst_eg[sq] + mobility_eg;
    } else {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= queen_pst_mg[flipsq] + mobility_mg;
        *egscore -= queen_pst_eg[flipsq] + mobility_eg;
    }

    eval_major_on_7th(pos, sq, mgscore, egscore);
}
