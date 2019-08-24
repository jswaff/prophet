#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <prophet/error_codes.h>
#include <prophet/parameters.h>

#include "command_internal.h"
#include "xboard/xboard_internal.h"

/* structure mapping a command to a function */
struct function_table_entry
{
    const char *name;
    cmd_func_t cmd_func;
};

/* a table of command->function mappings */
struct function_table_entry function_table[] = {

    /* xboard protocol */
    {"accepted", command_no_op},
    {"analyze", command_no_op},
    {"bk", command_no_op},
    {"computer", command_no_op},
    {"cores", command_no_op},
    {"draw", command_no_op},
    {"easy", command_no_op},
    {"force", xboard_force},
    {"go", xboard_go},
    {"hard", command_no_op},
    {"hint", command_no_op},
    {"ics", command_no_op},
    {"level", command_no_op},
    {"memory", command_no_op},
    {"name", command_no_op},
    {"new", xboard_new},
    {"nopost", command_no_op},
    {"nps", command_no_op},
    {"otim", command_no_op},
    {"ping", xboard_ping},
    {"playother", command_no_op},
    {"post", command_no_op},
    {"protover", xboard_protover},
    {"quit", command_exit},
    {"random", command_no_op},
    {"rating", command_no_op},
    {"rejected", command_no_op},
    {"remove", command_no_op},
    {"result", xboard_result},
    {"sd", command_no_op},
    {"setboard", command_no_op},
    {"st", command_no_op},
    {"time", command_no_op},
    {"undo", command_no_op},
    {"usermove", xboard_usermove},
    {"variant", command_no_op},
    {"xboard", command_no_op},
    {"?", command_no_op},

    /* custom commands */
    {"db", command_db},
    {"eval", command_no_op}, 
    {"exit", command_exit},
    {"perft", command_perft}
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
 * \return 0 on successful execution, and non-zero on failure.
 */
int parse_command(user_command_t* cmd, const char* input)
{
    int retval = 0;

    /* set the command */
    cmd->cmd = strdup(input);
    if (NULL == cmd->cmd)
    {
        return P4_ERROR_CMD_PARSE_CMD_COPY;
    }    

    /* set the function */
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
