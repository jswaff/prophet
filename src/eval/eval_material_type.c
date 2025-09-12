#include "eval_internal.h"

#include "position/position.h"


/**
 * \brief Evaluate the position's "material type".
 *
 * \param pos           a pointer to a chess position
 * \param draw_flag     a pointer to an integer to set the IMMEDIATE_DRAW flag
 *
 * \return the material type
 */
material_type_t eval_material_type(const position_t* pos, int* draw_flag)
{
    int num_white_pawns = pos->piece_counts[WHITE][PAWN];
    int num_white_knights = pos->piece_counts[WHITE][KNIGHT];
    int num_white_bishops = pos->piece_counts[WHITE][BISHOP];
    int num_white_rooks = pos->piece_counts[WHITE][ROOK];
    int num_white_queens = pos->piece_counts[WHITE][QUEEN];
    int num_white_minors = num_white_knights + num_white_bishops;
    int num_white_majors = num_white_rooks + num_white_queens;
    int num_white_total = num_white_pawns + num_white_minors +
        num_white_majors;

    int num_black_pawns = pos->piece_counts[BLACK][PAWN];
    int num_black_knights = pos->piece_counts[BLACK][KNIGHT];
    int num_black_bishops = pos->piece_counts[BLACK][BISHOP];
    int num_black_rooks = pos->piece_counts[BLACK][ROOK];
    int num_black_queens = pos->piece_counts[BLACK][QUEEN];
    int num_black_minors = num_black_knights + num_black_bishops;
    int num_black_majors = num_black_rooks + num_black_queens;
    int num_black_total = num_black_pawns + num_black_minors +
        num_black_majors;

    *draw_flag = 0;
    if (num_white_total > 2 || num_black_total > 2) return OTHER;

    material_type_t retval = OTHER;
    if (num_white_total == 0) {
        if (num_black_total == 0) {
             retval = KK;
        } else if (num_black_total == 1) {
            if (num_black_knights == 1) {
                retval = KKN;
            } else if (num_black_bishops == 1) {
                retval = KKB;
            }
        } else if (num_black_total == 2) {
            if (num_black_knights == 2) {
                retval = KKNN;
            }
        }
    } else if (num_white_total == 1) {
        if (num_white_pawns == 1) {
            if (num_black_total == 1) {
                if (num_black_knights == 1) {
                    retval = KPKN;
                } else if (num_black_bishops == 1) {
                    retval = KPKB;
                }
            }
        } else if (num_white_knights == 1) {
            if (num_black_total == 0) {
                retval = KNK;
            } else if (num_black_total == 1) {
                if (num_black_pawns == 1) {
                    retval = KNKP;
                } else if (num_black_knights == 1) {
                    retval = KNKN;
                } else if (num_black_bishops == 1) {
                    retval = KNKB;
                }
            }
        } else if (num_white_bishops == 1) {
            if (num_black_total == 0) {
                retval = KBK;
            } else if (num_black_total == 1) {
                if (num_black_pawns == 1) {
                    retval = KBKP;
                } else if (num_black_knights == 1) {
                    retval = KBKN;
                } else if (num_black_bishops == 1) {
                    retval = KBKB;
                }
            }
        }
    } else if (num_white_total == 2) {
        if (num_white_knights == 2) {
            if (num_black_total == 0) {
                retval = KNNK;
            }
        }
    }

    if ((!num_white_pawns && !num_white_majors && num_white_minors < 2 && 
        !num_black_pawns && !num_black_majors && num_black_minors < 2) || 
        retval == KKNN || retval == KNNK)
    {
        *draw_flag = 1;
    }

    return retval;
}