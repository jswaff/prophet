#include <prophet/error_codes.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

uint32_t time_remaining_millis;

extern bool fixed_time_per_move;
extern uint32_t max_time_ms;
extern uint32_t time_control_increment;


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
    int32_t time_remaining;
    if (1 != sscanf(input + 5, "%d", &time_remaining))
    {
        return P4_ERROR_CMD_XBOARD_TIME_MISSING_PARAMETER;
    }

    /* set the time remaining - centis to millis */
    if (time_remaining < 0) time_remaining = 0;
    time_remaining_millis = (uint32_t)time_remaining * 10;

    /* set the maximum search time */
    if (fixed_time_per_move)
    {
        /* leave 50ms for overhead */
        if (time_remaining_millis >= 50)
        {
            max_time_ms = time_remaining_millis - 50;
        }
        else
        {
            max_time_ms = 0;
        }
    }
    else
    {
        /* the basic strategy is to use 1/25th of the time plus increment */
        max_time_ms = time_remaining_millis / 25 + 
            time_control_increment * 1000;
    }

    /* success */
    return 0;
}
