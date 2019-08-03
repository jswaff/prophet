#include <stdbool.h>

#include <prophet/position/move.h>

/**
 * Test if a move is a member of a list.
 *
 * The score portion of the move is ignored.
 */
bool is_in_move_list(move mv, const move* start, const move* end)
{
    for (const move* mp=start; mp<end; mp++) 
    {
        if (clear_score(*mp) == clear_score(mv)) 
        {
            return true;
        }
    }

    return false;
}
