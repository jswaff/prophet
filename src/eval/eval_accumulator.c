#include <prophet/bitmap.h>

#include "eval_internal.h"

#include  <assert.h>


/**
 * \brief Accumulate the score for a set of pieces.
 *
 * \param pos           a pointer to a chess position
 * \param piece_map     a set of pieces to evaluate
 * \param endgame       if the eval should be done in the endgame phase 
 * \param eval_func     the evaluation function to use for each piece
 *
 * \return the cumulative score.
 */
int32_t eval_accumulator(const position_t* pos, uint64_t piece_map, bool endgame, eval_func_t eval_func)
{
    int32_t score = 0;

    while (piece_map)
    {
        square_t sq = (square_t)get_lsb(piece_map);

        score += eval_func(pos, sq, endgame);

        piece_map ^= square_to_bitmap(sq);
    }

    return score;
}
