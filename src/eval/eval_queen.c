#include <prophet/parameters.h>

#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a single queen.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square the queen is on
 * \param mgscore       a pointer to the middle game score accumulator
 * \param egscore       a pointer to the endgame score accumulator
 *
 */
void eval_queen(const position_t* pos, square_t sq, int32_t* mgscore, int32_t* egscore)
{
    assert(pos->piece[sq] == QUEEN || pos->piece[sq] == -QUEEN);

    int32_t s = eval_major_on_7th(pos, sq);

    if (is_white_piece(pos->piece[sq]))
    {
        *mgscore += queen_pst[sq] + s;
        *egscore += queen_endgame_pst[sq] + s;
    }
    else
    {
        int32_t flipsq = flip_rank[sq];
        *mgscore -= queen_pst[flipsq] + s;
        *egscore -= queen_endgame_pst[flipsq] + s;
    }

}
