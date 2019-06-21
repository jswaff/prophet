#include <stdlib.h>
#include <string.h>

#include "command_internal.h"

int parse_and_execute(const char* input, int* exit_status)
{
    int retval = 0;

    user_command_t* user_cmd = (user_command_t*)malloc(sizeof(user_command_t));

    if (NULL == user_cmd)
    {
        return 1;
    }

    // clear the structure
    memset(user_cmd, 0, sizeof(user_command_t));

    if (0 != parse_command(user_cmd, input))
    {
        retval = 1;
        goto cleanup_user_command;
    }

    // execute the command
    retval = user_cmd->cmd_func(user_cmd, exit_status);


cleanup_user_command:
    free(user_cmd->base_cmd);
    free(user_cmd);

    return retval;
}
