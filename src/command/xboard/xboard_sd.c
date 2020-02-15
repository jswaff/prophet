#include <prophet/error_codes.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int32_t max_depth;

/**
 * \brief Execute the xboard sd command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_sd(const char* input)
{
    /* verify the command */
    if (0 != strncmp("sd", input, 2))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 4)
    {
        return P4_ERROR_CMD_XBOARD_SD_MISSING_DEPTH;
    }

    /* attempt to read the DEPTH parameter */
    int depth;
    if (1 != sscanf(input + 3, "%d", &depth))
    {
        return P4_ERROR_CMD_XBOARD_SD_MISSING_DEPTH;
    }

    /* set the search depth */
    max_depth = depth;

    /* success */
    return 0;
}
