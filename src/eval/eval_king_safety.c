#include <prophet/parameters.h>

#include "eval_internal.h"


/**
 * \brief Evaluate king safety for one side of the board.
 *
 * \param pos           a pointer to a chess position
 * \param wtm           white to move?
 *
 * \return a score of the king safety for the player specified.
 */
int32_t eval_king_safety(const position_t* UNUSED(pos), bool wtm)
{

    if (wtm)
    {

    }
    else /* black to move */
    {

    }

    return 0;
}