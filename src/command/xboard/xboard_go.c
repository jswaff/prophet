#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <string.h>


extern bool xboard_force_mode;


int xboard_go(const char *input)
{
    /* verify the command */
    if (0 != strcmp("go", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = false;

    return think_and_make_move();
}
