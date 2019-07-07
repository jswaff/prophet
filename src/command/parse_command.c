#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <prophet/error_codes.h>
#include <prophet/parameters.h>

#include "command_internal.h"
#include "xboard/xboard-internal.h"

/* structure mapping a command to a function */
struct function_table_entry
{
    const char *name;
    cmd_func_t cmd_func;
};

/* a table of command->function mappings */
struct function_table_entry function_table[] = {

    /* xboard protocol */
    {"xboard", command_no_op},
    {"ping", xboard_ping},
    {"quit", command_exit},

    /* custom commands */
    {"db", command_db},
    {"perft", command_perft},
    {"exit", command_exit}
};

/**
 * \brief Parse a command
 * 
 * Parse the user input and attempt to map it to a command.  Note that failure
 * to map the input to a command does not generate a failure return code.  In 
 * this case the input is mapped to the no-op command.
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
        return P4_ERROR_CMD_PARSE_CMD_COPY;
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

    printf("unknown command: %s\n", input);
    cmd->cmd_func = &command_no_op;

done:
    return retval;
}
