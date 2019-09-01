#include <prophet/bitmap.h>

#include "eval_internal.h"


/**
 * \brief Evaluate king safety for one side of the board.
 *
 * \param pos           a pointer to a chess position
 * \param wtm           white to move?
 *
 * \return a score of the king safety for the player specified.
 */
int32_t eval_king_safety(const position_t* pos, bool wtm)
{
    int32_t score = 0;

    if (wtm)
    {
        file_t file_king = get_file(pos->white_king);

        /* king side */
        if (file_king > FILE_E)
        {

        }
        /* queen side */
        else if (file_king < FILE_D)
        {

        }
        /* middle */
        else
        {
            if (! ((pos->white_pawns | pos->black_pawns) & 
                sq_to_file_bitmap(pos->white_king))) 
            {
                score += king_safety_middle_open_file;
            }
        }
    }
    else /* black to move */
    {
        file_t file_king = get_file(pos->black_king);

        /* king side */
        if (file_king > FILE_E)
        {

        }
        /* queen side */
        else if (file_king < FILE_D)
        {

        }
        /* middle */
        else
        {
            if (! ((pos->white_pawns | pos->black_pawns) & 
                sq_to_file_bitmap(pos->black_king))) 
            {
                score += king_safety_middle_open_file;
            }
        }
    }

    return score;
}