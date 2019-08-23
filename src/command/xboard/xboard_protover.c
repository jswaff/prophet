#include <stdio.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

/**
 * \brief Execute the xboard protover command 
 * 
 * Respond to the protover command by printing a list of supported features.  
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_protover(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strncmp("protover", input, 8))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 10)
    {
        return P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N;   
    }

    int n;

    /* advance the base command pointer past the "protover " substring */
    if (1 != sscanf(input + 9, "%d", &n))
    {
        return P4_ERROR_CMD_XBOARD_PROTOVER_MISSING_N;
    }

    /* this program support protocol version 2 and higher */
    if (n < 2)
    {
        *exit_status = 1;
        return P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED;
    }

    /* print the features that are supported and not supported */
    printf("feature analyze=0 black=0 colors=0 cores=0 ping=1 draw=0 debug=1 "
        "edit=0 ics=0 level=0 name=0 nps=0 memory=0 playother=0 pause=0 "
        "resume=0 reuse=1 san=0 setboard=1 sigint=0 sigterm=0 smp=0 st=0 "
        "time=1 usermove=1 white=0 variants=\"normal\" done=1\n");

    *exit_status = 0;

    return 0;
}
