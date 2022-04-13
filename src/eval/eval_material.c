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

    /*int num_pawns = pos->piece_counts[color][PAWN];*/
    int num_knights = pos->piece_counts[color][KNIGHT];
    int num_bishops = pos->piece_counts[color][BISHOP];
    int num_rooks = pos->piece_counts[color][ROOK];
    int num_queens = pos->piece_counts[color][QUEEN];

    /* raise the knight's value 1/16 for each pawn above 5, and lower for each
     * pawn below 5.
     */
    int32_t knight_adj = 0; /*(num_pawns - 5) * 6;*/

    /* lower the rook's value 1/8 for each pawn above 5, and raise for each
     * pawn below 5.
     */
    int32_t rook_adj = 0; /*(num_pawns - 5) * -12;*/

    return num_rooks * (rook_val + rook_adj) +
        num_knights * (knight_val + knight_adj) +
        num_bishops * bishop_val + (num_bishops > 1 ? bishop_pair : 0) +
        num_queens * queen_val;
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

