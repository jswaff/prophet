#include <stdio.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

/**
 * \brief Execute the exit command.
 * 
 * \param input         The user entered input
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int command_exit(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strcmp("exit", input) && 0 != strcmp("quit", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    printf("bye.\n");

    *exit_status = 1;

    return 0;
}

