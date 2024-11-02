#include "eval_internal.h"

#include "prophet/position.h"
#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "movegen/movegen_internal.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * \brief Evaluate a single bishop.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the bishop is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_bishop(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == BISHOP || pos->piece[sq] == -BISHOP);

    uint64_t empty_squares = ~(pos->white_pieces | pos->black_pieces);
    uint32_t mobility = popcnt(get_bishop_moves(pos, sq, empty_squares));
    uint32_t mobility_mg = bishop_mobility_mg[mobility];
    uint32_t mobility_eg = bishop_mobility_eg[mobility];

    if (is_white_piece(pos->piece[sq])) {
        *mgscore += bishop_pst_mg[sq] + mobility_mg;
        *egscore += bishop_pst_eg[sq] + mobility_eg;
        if (trapped_bishop(pos, sq, true)) {
            *mgscore += bishop_trapped;
            *egscore += bishop_trapped;
        }
    } else {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= (bishop_pst_mg[flipsq] + mobility_mg);
        *egscore -= (bishop_pst_eg[flipsq] + mobility_eg);
        if (trapped_bishop(pos, sq, false)) {
            *mgscore -= bishop_trapped;
            *egscore -= bishop_trapped;
        }
    }
}


/**
 * \brief Evaluate the position for bishop pairs.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a score for the bishop pair.
 */
int32_t eval_bishop_pair(const position_t* pos)
{
    return (popcnt(pos->white_bishops) > 1 ? bishop_pair : 0) -
        (popcnt(pos->black_bishops) > 1 ? bishop_pair : 0);
}