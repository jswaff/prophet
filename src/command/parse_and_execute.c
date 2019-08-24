#include <prophet/error_codes.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_internal.h"

/**
 * \brief Parse the user input into a command and execute the command.
 *
 * Note that failure to execute a command does not yield an error return code.
 * For this reason, it's important to check both the return code and the exit
 * status.
 *
 * \param input         the user entered input
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int parse_and_execute(const char* input, int* exit_status)
{
    int retval = 0;

    user_command_t* user_cmd = (user_command_t*)malloc(sizeof(user_command_t));

    if (NULL == user_cmd)
    {
        return P4_ERROR_CMD_USER_CMD_BUFFER;
    }

    /* clear the structure */
    memset(user_cmd, 0, sizeof(user_command_t));

    retval = parse_command(user_cmd, input);
    if (0 != retval)
    {
        goto cleanup_user_command;
    }

    /* execute the command */
    int cmd_retval = user_cmd->cmd_func(input, exit_status);
    if (0 != cmd_retval)
    {
        /* TODO: translate error code to text descriptions */
        printf("Error (invocation failed with code %d): %s\n", cmd_retval, input);
    }

cleanup_user_command:
    
    free(user_cmd->cmd);
    free(user_cmd);

    return retval;
}
