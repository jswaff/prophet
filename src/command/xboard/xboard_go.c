#include <prophet/error_codes.h>

#include <string.h>

#include "xboard_internal.h"

extern bool xboard_force_mode;

/**
 * \brief Execute the xboard go command 
 * 
 * Leave force mode and set the engine to play the color that is on move. 
 * Associate the engine's clock with the color that is on move, the opponent's 
 * clock with the color that is not on move. Start the engine's clock. Start 
 * thinking and eventually make a move.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_go(const char* input)
{
    /* verify the command */
    if (0 != strcmp("go", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = false;

    return think_and_make_move();
}
