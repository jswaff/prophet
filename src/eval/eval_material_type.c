#include "eval_internal.h"

/**
 * \brief Evaluate the position's "material type".
 *
 * \param pos           a pointer to a chess position
 *
 * \return the material type
 */
material_type_t eval_material_type(const position_t* pos)
{
    int num_white_pawns = pos->piece_counts[WHITE][PAWN];
    int num_white_knights = pos->piece_counts[WHITE][KNIGHT];
    int num_white_bishops = pos->piece_counts[WHITE][BISHOP];
    int num_white_rooks = pos->piece_counts[WHITE][ROOK];
    int num_white_queens = pos->piece_counts[WHITE][QUEEN];
    int num_white_total = num_white_pawns + num_white_knights +
        num_white_bishops + num_white_rooks + num_white_queens;

    int num_black_pawns = pos->piece_counts[BLACK][PAWN];
    int num_black_knights = pos->piece_counts[BLACK][KNIGHT];
    int num_black_bishops = pos->piece_counts[BLACK][BISHOP];
    int num_black_rooks = pos->piece_counts[BLACK][ROOK];
    int num_black_queens = pos->piece_counts[BLACK][QUEEN];
    int num_black_total = num_black_pawns + num_black_knights +
        num_black_bishops + num_black_rooks + num_black_queens;

    if (num_white_total > 2 || num_black_total > 2) return OTHER;

    if (num_white_total + num_black_total == 0) return KK;

    if (num_white_total == 0)
    {
        if (num_black_total == 1)
        {
            if (num_black_knights == 1) return KNK;
            if (num_black_bishops == 1) return KBK;
        }
    }
    else if (num_white_total == 1)
    {
        if (num_white_bishops == 1)
        {
            if (num_black_total == 1 && num_black_pawns == 1) 
            {
                return KBKP;
            }
        }
        else if (num_white_knights == 1)
        {
            if (num_black_total == 1 && num_black_pawns == 1)
            {
                return KNKP;
            }
        }
    }


    if (num_black_total == 0)
    {
        if (num_white_total == 1)
        {
            if (num_white_knights == 1) return KNK;
            if (num_white_bishops == 1) return KBK;
        }
    }
    else if (num_black_total == 1)
    {
        if (num_black_bishops == 1)
        {
            if (num_white_total == 1 && num_white_pawns == 1)
            {
                return KBKP;
            }
        }
        else if (num_black_knights == 1)
        {
            if (num_white_total == 1 && num_white_pawns == 1)
            {
                return KNKP;
            }
        }
    }

    return OTHER;
}