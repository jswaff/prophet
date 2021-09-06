#include <prophet/bitmap.h>

#include "eval_internal.h"


/**
 * \brief Evaluate a position for draw by lack of mating material.
 *
 * \param pos           a pointer to a chess position
 *
 * \return whether the position is drawn or not.
 */
bool eval_draw(const position_t* pos)
{
    uint32_t num_white_pieces = popcnt(pos->white_pieces);
    uint32_t num_black_pieces = popcnt(pos->black_pieces);

    if (num_white_pieces==1 && num_black_pieces==1)
    {
        return true;
    }

    if (num_white_pieces > 3 || num_black_pieces > 3)
    {
        return false;
    }

    if (num_white_pieces == 1)
    {
        if (num_black_pieces == 2)
        {
            if (pos->piece_counts[BLACK][KNIGHT] > 0 || pos->piece_counts[BLACK][BISHOP] > 0)
            {
                return true;
            }
        }
        else 
        {
            if (pos->piece_counts[BLACK][KNIGHT] == 2)
            {
                return true;
            }
        }
    }
    else if (num_black_pieces == 1)
    {
        if (num_white_pieces == 2)
        {
            if (pos->piece_counts[WHITE][KNIGHT] > 0 || pos->piece_counts[WHITE][BISHOP] > 0)
            {
                return true;
            }
        }
        else
        {
            if (pos->piece_counts[WHITE][KNIGHT] == 2)
            {
                return true;
            }
        }
    }
    else if (num_white_pieces == 2 && num_black_pieces == 2)
    {
        if ((pos->piece_counts[WHITE][KNIGHT] > 0 || pos->piece_counts[WHITE][BISHOP] > 0) &&
            (pos->piece_counts[BLACK][KNIGHT] > 0 || pos->piece_counts[BLACK][BISHOP] > 0))
        {
            return true;
        }
    }

    return false;
}
