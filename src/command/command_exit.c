#include <prophet/error_codes.h>

#include <stdio.h>
#include <string.h>

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

    printf("bye.\n");

    return 0;
}

