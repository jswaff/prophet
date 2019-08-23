#include <stdio.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

/**
 * \brief Execute the xboard result command 
 * 
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int xboard_result(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strncmp("result", input, 6))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
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
  
    *exit_status = 0;

    return 0;
}
