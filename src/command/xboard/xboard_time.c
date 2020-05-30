#include <prophet/error_codes.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t time_remaining_millis;

/**
 * \brief Execute the xboard time command 
 * 
 * Respond to the time command by setting the time remaining.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_time(const char* input)
{
    /* verify the command */
    if (0 != strncmp("time", input, 4))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 6)
    {
        return P4_ERROR_CMD_XBOARD_TIME_MISSING_PARAMETER;
    }

    /* attempt to read the time remaining parameter */
    uint32_t time_remaining;
    if (1 != sscanf(input + 5, "%d", &time_remaining))
    {
        return P4_ERROR_CMD_XBOARD_TIME_MISSING_PARAMETER;
    }

    /* set the time remaining - centis to millis */
    time_remaining_millis = time_remaining * 10;

    /* success */
    return 0;
}
