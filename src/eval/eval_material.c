#include "eval_internal.h"

#include  <assert.h>

/**
 * \brief Evaluate the position's non-pawn material.
 *
 * \param pos           a pointer to a chess position
 * \param for_white     whether the returned value should be for white
 *
 * \return the score.
 */
int32_t eval_nonpawn_material(const position_t* pos, bool for_white)
{
    int color = for_white ? WHITE : BLACK;

    int num_bishops = pos->piece_counts[color][BISHOP];

    return pos->piece_counts[color][ROOK] * rook_val +
        pos->piece_counts[color][KNIGHT] * knight_val +
        num_bishops * bishop_val +
        pos->piece_counts[color][QUEEN] * queen_val +
        (num_bishops > 1 ? bishop_pair : 0);
}


/**
 * \brief Evaluate the position's pawn material.
 *
 * \param pos           a pointer to a chess position
 * \param for_white     whether the returned value should be for white
 *
 * \return the score.
 */
int32_t eval_pawn_material(const position_t* pos, bool for_white)
{
    int color = for_white ? WHITE : BLACK;

    return pos->piece_counts[color][PAWN] * pawn_val;
}

