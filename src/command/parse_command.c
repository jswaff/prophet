#include "command.h"

#include "prophet/error_codes.h"

#include "util/output.h"
#include "xboard/xboard_internal.h"

#include <stdbool.h>
#include <string.h>


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
    {"bk", command_no_op},
    {"computer", command_no_op},
    {"easy", command_no_op},
    {"force", xboard_force},
    {"go", xboard_go},
    {"hard", command_no_op},
    {"hint", command_no_op},
    {"level", xboard_level},
    {"memory", xboard_memory},
    {"new", xboard_new},
    {"nopost", xboard_nopost},
    {"otim", command_no_op},
    {"ping", xboard_ping},
    {"post", xboard_post}, 
    {"protover", xboard_protover},
    {"quit", xboard_quit},
    {"random", command_no_op},
    {"rating", command_no_op},
    {"rejected", command_no_op},
    {"remove", xboard_remove},
    {"result", xboard_result},
    {"sd", xboard_sd},
    {"setboard", xboard_setboard},
    {"st", xboard_st},
    {"time", xboard_time},
    {"undo", xboard_undo},
    {"usermove", xboard_usermove},
    {"xboard", command_no_op},
    {"?", xboard_move_now},

    /* custom commands */
    {"db", command_db},
    {"eval", command_eval}, 
    {"exit", xboard_quit},
    {"perft", command_perft}
};

/**
 * \brief Parse a command
 * 
 * Parse the user input and attempt to map it to a command.  Note that failure
 * to map the input to a command does not generate a failure return code.  In 
 * this case the input is mapped to the no-op command.
 *
 * Implementation note: for the most part the commands specific to XBoard are 
 * isolated in the xboard folder.  Some of the XBoard logic has leaked up, but
 * it's pretty minimal and wouldn't be hard to refactor if support for another
 * protocol were added in the future.
 *
 * \param cmd           pointer to structure to receive parsed command
 * \param input         buffer containing command to be parsed
 * \param exit_status   pointer to boolean to receive exit status
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int parse_command(user_command_t* cmd, const char* input, bool* exit_status)
{
    int retval = 0;
    *exit_status = false;

    /* set the command */
    cmd->cmd = strdup(input);
    if (NULL == cmd->cmd) {
        return P4_ERROR_CMD_PARSE_CMD_COPY;
    }    

    /* set the function */
    int nfuncs = sizeof(function_table) / sizeof(struct function_table_entry);

    for (int i=0; i<nfuncs; i++) {
        struct function_table_entry fte = function_table[i];
        if (strlen(input) >= strlen(fte.name) &&
            !strncmp(fte.name, input, strlen(fte.name))) 
        {
            cmd->cmd_func = fte.cmd_func;
            goto done;
        }
    }

    /* if string is move only per xboard protocol 1*/
    if (strlen(input)>=4 && strlen(input)<=5) {
        cmd->cmd_func = xboard_move;
        goto done;
    }


    /* Note - this error message is part of the xboard protocol */
    plog("Error (unknown command): %s\n", input);
    cmd->cmd_func = &command_no_op;

done:

    /* special case - if the command was mapped to the exit handler, set the
     * exit_status flag. */
    if (cmd->cmd_func == xboard_quit) {
        *exit_status = true;
    }

    return retval;
}
