#include <prophet/bitmap.h>
#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

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

    if (is_white_piece(pos->piece[sq]))
    {
        *mgscore += bishop_pst[sq];
        *egscore += bishop_endgame_pst[sq];
    }
    else
    {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= bishop_pst[flipsq];
        *egscore -= bishop_endgame_pst[flipsq];
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