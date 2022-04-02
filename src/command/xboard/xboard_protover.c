#include <prophet/error_codes.h>
#include <prophet/util/output.h>

#include <string.h>

/**
 * \brief Execute the xboard protover command 
 * 
 * Respond to the protover command by printing a list of supported features.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_protover(const char* input)
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
    /*if (n < 2)
    {
        return P4_ERROR_CMD_XBOARD_PROTOVER_UNSUPPORTED;
    }*/

    /* print the features that are supported and not supported */
    plog(
        "feature analyze=0 colors=0 draw=0 debug=1 "
        "memory=1 myname=\"Prophet4\" name=0 nps=0 "
        "ping=1 setboard=1 sigint=0 sigterm=0 usermove=1 "
        "variants=\"normal\" done=1\n");

    return 0;
}
