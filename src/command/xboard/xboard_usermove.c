#include <stdio.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/position/move.h>
#include <prophet/util/string_utils.h>
#include <prophet/parameters.h>

extern position gpos;
extern bool xboard_force_mode;

/**
 * \brief Execute the xboard usermove command 
 * 
 * If the move is illegal, print an error message; see the section "Commands 
 * from the engine to xboard". If the move is legal and in turn, make it. If 
 * not in force mode, stop the opponent's clock, start the engine's clock, 
 * start thinking, and eventually make a move. 
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_usermove(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strncmp("usermove", input, 8))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? 
     * the shortest possible valid command would hold a two character
     * move, e.g. "usermove e4"
     */
    if (strlen(input) < 11)
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE;   
    }

    /* read the move */
    char str_mv[10];
    if (1 != sscanf(input + 10, "%s", str_mv))
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* attempt to parse the move */
    move mv = str_to_move(str_mv);
    if (BADMOVE == mv)
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* TODO: apply move */

    /* TODO: if not in force mode, start thinking ... */
  
    *exit_status = 0;

    return 0;
}
