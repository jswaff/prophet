#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern volatile uint32_t max_depth;


int xboard_sd(const char *input)
{
    /* verify the command */
    if (0 != strncmp("sd", input, 2)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 4) {
        return ERROR_CMD_XBOARD_SD_MISSING_DEPTH;
    }

    /* attempt to read the DEPTH parameter */
    uint32_t depth;
    if (1 != sscanf(input + 3, "%d", &depth)) {
        return ERROR_CMD_XBOARD_SD_MISSING_DEPTH;
    }

    /* set the search depth */
    max_depth = depth;

    /* success */
    return 0;
}
