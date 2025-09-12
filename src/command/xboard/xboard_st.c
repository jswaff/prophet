#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


extern volatile uint32_t max_time_ms;
extern bool fixed_time_per_move;


/**
 * @brief Execute the xboard st command 
 * 
 * Respond to the st TIME command by setting the maximum search time
 *
 * @param input         the user entered input 
 *
 * @return 0 on successful execution, and non-zero on failure
 */
int xboard_st(const char* input)
{
    /* verify the command */
    if (0 != strncmp("st", input, 2)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 4) {
        return ERROR_CMD_XBOARD_ST_MISSING_DEPTH;
    }

    /* attempt to read the TIME parameter */
    uint32_t t;
    if (1 != sscanf(input + 3, "%d", &t)) {
        return ERROR_CMD_XBOARD_ST_MISSING_DEPTH;
    }

    /* set the search time - seconds to milliseconds */
    max_time_ms = t * 1000;

    /* we should NOT calculate time to use before each move */
    fixed_time_per_move = true;

    /* success */
    return 0;
}
