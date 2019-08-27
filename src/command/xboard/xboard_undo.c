#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/position/position.h>

#include <stdio.h>
#include <string.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];


/**
 * \brief Execute the xboard undo command 
 * 
 * Undo the previous move.  XBoard protocol guarantees that the engine will be
 * in force mode before invoking this command.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_undo(const char* input)
{
    /* verify the command */
    if (0 != strcmp("undo", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    if (gpos.move_counter < 1)
    {
        return P4_ERROR_GUNDO_INDEX_LB_VIOLATION;
    }

    undo_move(&gpos, gundos + gpos.move_counter - 1);

    return 0;
}
