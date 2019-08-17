#include <stdbool.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

extern bool xboard_force_mode;

/**
 * \brief Execute the xboard go command 
 * 
 * Leave force mode and set the engine to play the color that is on move. 
 * Associate the engine's clock with the color that is on move, the opponent's 
 * clock with the color that is not on move. Start the engine's clock. Start 
 * thinking and eventually make a move.
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_go(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strcmp("go", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = false;

    /* TODO: start search */

    *exit_status = 0;

    return 0;
}

