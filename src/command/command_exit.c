#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <string.h>

#include "xboard/xboard_internal.h"

/* TODO: this should be in the xboard folder */

/**
 * \brief Execute the exit command.
 * 
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_exit(const char* input)
{
    /* verify the command */
    if (0 != strcmp("exit", input) && 0 != strcmp("quit", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    stop_search_thread_blocking();
    
    out(stdout, "bye.\n");

    return 0;
}

