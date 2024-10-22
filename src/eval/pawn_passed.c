#include <prophet/bitmap.h>
#include <prophet/position/piece.h>

#include  <assert.h>

#include "eval_internal.h"

static uint64_t bb_passed[64][2];

/**
 * \brief Determine if a pawn is passed.
 *
 * An passed pawn is a pawn that no enemy pawn can stop from queening.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if the pawn is passed, otherwise false.
 */
bool pawn_passed(const position_t* pos, square_t pawn_sq)
{
    assert(pos->piece[pawn_sq] == PAWN || pos->piece[pawn_sq] == -PAWN);

    if (is_white_piece(pos->piece[pawn_sq])) {
        return !(bb_passed[pawn_sq][WHITE] & pos->black_pawns);
    }
    else /* black pawn */
    {
        return !(bb_passed[pawn_sq][BLACK] & pos->white_pawns);
    }
}


/**
 * \brief Initialize the pawn_passed function.
 */
void init_pawn_passed()
{
    for (int i=0; i<64; i++) {
        file_t f = get_file(i);

        bb_passed[i][WHITE] = ray(i, NORTH);
        bb_passed[i][BLACK] = ray(i, SOUTH);

        if (f > FILE_A) {
            bb_passed[i][WHITE] |= ray(i-1, NORTH);
            bb_passed[i][BLACK] |= ray(i-1, SOUTH);
        }
		
        if (f < FILE_H) {
            bb_passed[i][WHITE] |= ray(i+1, NORTH);
            bb_passed[i][BLACK] |= ray(i+1, SOUTH);
        }
    }
}
