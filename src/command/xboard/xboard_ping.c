#include <stdio.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

/**
 * \brief Execute the xboard ping command 
 * 
 * Respond to the ping N command with a pong N.  
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_ping(const char* input, int* exit_status)
{
    *exit_status = 0;

    int n;

    /* advance the base command pointer past the "ping " substring */
    if (1 != sscanf(input + 5, "%d", &n))
    {
        return P4_ERROR_CMD_XBOARD_PING_MISSING_N;
    }

    printf("pong %d\n", n);
    
    return 0;
}

