#include "eval_internal.h"

#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>


void eval_accumulator(const position_t *pos, uint64_t piece_map, int32_t *mgscore, int32_t *egscore, 
	eval_func_t eval_func)
{
    while (piece_map) {
        square_t sq = (square_t)get_lsb(piece_map);

        eval_func(pos, sq, mgscore, egscore);

        piece_map ^= square_to_bitmap(sq);
    }
}
