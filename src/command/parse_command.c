#include <string.h>

#include <prophet/parameters.h>

#include "command_internal.h"

/* structure mapping a command to a function */
struct function_table_entry
{
    const char *name;
    cmd_func_t cmd_func;
};

/* a table of command->function mappings */
struct function_table_entry function_table[] = {
    {"perft", command_perft},
    {"exit", command_exit}
};

/**
 * \brief Parse a command
 * 
 * \param cmd           Pointer to structure to receive parsed command
 * \param input         buffer containing command to be parsed
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int parse_command(user_command_t* cmd, const char* input)
{
    int retval = 0;

    // set the command
    cmd->cmd = strdup(input);
    if (NULL == cmd->cmd)
    {
        return 1;
    }    

    // set the function
    int nfuncs = sizeof(function_table) / sizeof(struct function_table_entry);

    for (int i=0; i<nfuncs; i++)
    {
        struct function_table_entry fte = function_table[i];
        if (strlen(input) >= strlen(fte.name) &&
            !strncmp(fte.name, input, strlen(fte.name)))
        {
            cmd->cmd_func = fte.cmd_func;
            goto done;
        }
    }


    // unknown - map to no-op
    if (NULL == cmd->cmd_func)
    {
        cmd->cmd_func = &command_no_op;
    }

done:
    return retval;
}
