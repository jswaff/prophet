#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/position/position.h>

#include <stdio.h>
#include <string.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern int gundo_ind;

/**
 * \brief Execute the xboard remove command 
 * 
 * Undo the previous FULL move - one move for each player.  XBoard protocol 
 * guarantees that this command will only be sent when the user is on move.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_remove(const char* input)
{
    /* verify the command */
    if (0 != strcmp("remove", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    if (gundo_ind < 2)
    {
        return P4_ERROR_GUNDO_INDEX_LB_VIOLATION;
    }

    gundo_ind--;
    undo_move(&gpos, gundos + gundo_ind);

    gundo_ind--;
    undo_move(&gpos, gundos + gundo_ind);

    return 0;
}
