#include <prophet/error_codes.h>

#include <stdio.h>
#include <string.h>


#include "xboard_internal.h"


/**
 * \brief Execute the xboard result command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_result(const char* input)
{
    /* verify the command */
    if (0 != strncmp("result", input, 6))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    int retval = stop_search_thread_blocking();
    if (0 != retval)
    {
        return retval;
    }

    /* is the command long enough to contain an argument? 
     * the shortest possible valid command is "result *"
     */
    if (strlen(input) < 8)
    {
        return P4_ERROR_CMD_XBOARD_RESULT_MISSING_RESULT;   
    }

    /* read the result, 1-0, 0-1, 1/2-1/2, or * */
    char result[8];
    if (1 != sscanf(input + 7, "%s", result))
    {
        return P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT;
    }

    if (!strcmp(result, "1-0")) 
    {
        /* TODO */
    }
    else if (!strcmp(result, "0-1")) 
    {
        /* TODO */
    }
    else if (!strcmp(result, "1/2-1/2")) 
    {
        /* TODO */
    }
    else if (!strcmp(result, "*")) 
    {
        /* TODO */
    }
    else 
    {
        return P4_ERROR_CMD_XBOARD_RESULT_INVALID_RESULT;   
    }
  
    return 0;
}
