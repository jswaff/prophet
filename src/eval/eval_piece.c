#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate a piece.
 *
 * \param piece         the piece to evaluate
 *
 * \return the score
 */
int32_t eval_piece(int32_t piece)
{
    const int32_t pvals[13] = 
    { 
        INF, queen_val, rook_val, bishop_val, knight_val, pawn_val, 0,
        pawn_val, knight_val, bishop_val, rook_val, queen_val, INF 
    };
    return pvals[piece+6];
}
