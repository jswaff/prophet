#include <prophet/bitmap.h>
#include <prophet/position/piece.h>

#include  <assert.h>

#include "eval_internal.h"

static uint64_t bb_pawn_attacked[64][2];


/**
 * \brief Determine if a pawn supports a sq.
 *
 * \param pos           a pointer to a chess position
 * \param pawn_sq       the pawn square
 *
 * \return true if a friendly pawn supports, otherwise false.
 */ 
bool pawn_supports(const position_t* pos, square_t sq)
{
    if (is_white_piece(pos->piece[sq])) {
        return bb_pawn_attacked[sq][BLACK] & pos->white_pawns;
    }
    else /* black pawn */
    {
        return bb_pawn_attacked[sq][WHITE] & pos->black_pawns;
    }
}


/**
 * \brief Initialize the pawn_supports function.
 */
void init_pawn_attacked()
{
    for (int i=0; i<64; i++) {
        file_t f = get_file(i);

        bb_pawn_attacked[i][WHITE] = 0;
        bb_pawn_attacked[i][BLACK] = 0;

        if (f > FILE_A) {
            if (get_rank(i) != RANK_8) {
                bb_pawn_attacked[i][WHITE] |= square_to_bitmap(i-9);
            }
            if (get_rank(i) != RANK_1) {
                bb_pawn_attacked[i][BLACK] |= square_to_bitmap(i+7);
            }
        }
        
        if (f < FILE_H) {
            if (get_rank(i) != RANK_8) {
                bb_pawn_attacked[i][WHITE] |= square_to_bitmap(i-7);
            }
            if (get_rank(i) != RANK_1) {
                bb_pawn_attacked[i][BLACK] |= square_to_bitmap(i+9);
            }
        }
    }
}
