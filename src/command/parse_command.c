#include <string.h>

#include "command_internal.h"

int parse_command(user_command_t* cmd, const char* input)
{
    int retval = 0;

    // TODO: split input up by spaces

    // TODO: consider using array of function pointers

    if (!strcmp("exit", input))
    {
        cmd->cmd_func = &command_exit;
    }


    // unknown - map to no-op
    if (NULL == cmd->cmd_func)
    {
        cmd->cmd_func = &command_no_op;
    }

    return retval;
}
