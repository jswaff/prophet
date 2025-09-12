#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <string.h>


int xboard_move_now(const char *input)
{
    /* verify the command */
    if (0 != strcmp("?", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    return block_on_search_thread(true);
}
