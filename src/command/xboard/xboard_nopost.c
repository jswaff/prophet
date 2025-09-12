#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <string.h>

extern bool xboard_post_mode;


int xboard_nopost(const char *input)
{
    /* verify the command */
    if (0 != strncmp("nopost", input, 6)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_post_mode = false;

    /* success */
    return 0;
}
