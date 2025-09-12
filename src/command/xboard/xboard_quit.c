#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include "util/output.h"

#include <string.h>


int xboard_quit(const char *input)
{
    /* verify the command */
    if (0 != strcmp("exit", input) && 0 != strcmp("quit", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    int retval = block_on_search_thread(true);

    plog("bye.\n");

    return retval;
}
