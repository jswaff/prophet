#include <prophet/error_codes.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>


uint32_t max_time_ms = 0;


/**
 * \brief Execute the xboard sd command 
 * 
 * Respond to the st TIME command by setting the maximum search time
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_st(const char* input)
{
    /* verify the command */
    if (0 != strncmp("st", input, 2))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 4)
    {
        return P4_ERROR_CMD_XBOARD_ST_MISSING_DEPTH;
    }

    /* attempt to read the TIME parameter */
    uint32_t t;
    if (1 != sscanf(input + 3, "%d", &t))
    {
        return P4_ERROR_CMD_XBOARD_ST_MISSING_DEPTH;
    }

    /* set the search time - seconds to milliseconds */
    max_time_ms = t * 1000;

    /* success */
    return 0;
}
