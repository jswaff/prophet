#include "eval_internal.h"

#include "prophet/piece.h"
#include "prophet/square.h"

#include "bitmap/bitmap.h"
#include "position/position.h"
#include "position/square_internal.h"

#include <assert.h>
#include <stdbool.h>

static uint64_t bb_outpost[64][2];


bool outpost(const position_t *pos, square_t sq, bool white_pov)
{
    if (white_pov) {
        return !(bb_outpost[sq][WHITE] & pos->black_pawns);
    } else {
        return !(bb_outpost[sq][BLACK] & pos->white_pawns);
    }
}


void init_outpost()
{
    for (int i=0; i<64; i++) {
        file_t f = get_file(i);

        bb_outpost[i][WHITE] = 0;
        bb_outpost[i][BLACK] = 0;

        if (f > FILE_A) {
            bb_outpost[i][WHITE] |= ray(i-1, NORTH);
            bb_outpost[i][BLACK] |= ray(i-1, SOUTH);
        }
        
        if (f < FILE_H) {
            bb_outpost[i][WHITE] |= ray(i+1, NORTH);
            bb_outpost[i][BLACK] |= ray(i+1, SOUTH);
        }
    }
}
