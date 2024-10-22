#include <prophet/bitmap.h>
#include <prophet/position/piece.h>

#include  <assert.h>

#include "eval_internal.h"

static uint64_t bb_outpost[64][2];

/**
 * \brief Determine if a square is an outpost.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the square
 * \param white_pov     whether to examine from white's POV
 *
 * \return true if the square is an outpost, otherwise false.
 */
bool outpost(const position_t* pos, square_t sq, bool white_pov)
{
    if (white_pov) {
        return !(bb_outpost[sq][WHITE] & pos->black_pawns);
    } else {
        return !(bb_outpost[sq][BLACK] & pos->white_pawns);
    }
}

/**
 * \brief Initialize the outpost function.
 */
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
