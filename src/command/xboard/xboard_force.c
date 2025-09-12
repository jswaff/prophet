#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <string.h>

bool xboard_force_mode;


int xboard_force(const char *input)
{
    /* verify the command */
    if (0 != strcmp("force", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = true;
    int retval = block_on_search_thread(true);

    return retval;
}

