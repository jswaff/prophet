#include "eval_internal.h"

#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>

/**
 * @brief Accumulate the score for a set of pieces.
 *
 * @param pos           a pointer to a chess position
 * @param piece_map     a set of pieces to evaluate
 * @param mgscore       a pointer to the middle game score accumulator
 * @param egscore       a pointer to the endgame score accumulator
 * @param eval_func     the evaluation function to use for each piece
 *
 */
void eval_accumulator(const position_t* pos, uint64_t piece_map, int32_t* mgscore, int32_t* egscore, 
	eval_func_t eval_func)
{
    while (piece_map) {
        square_t sq = (square_t)get_lsb(piece_map);

        eval_func(pos, sq, mgscore, egscore);

        piece_map ^= square_to_bitmap(sq);
    }
}
