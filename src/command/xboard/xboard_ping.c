#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include "util/output.h"

#include <stdbool.h>
#include <string.h>


int xboard_ping(const char *input)
{
    /* verify the command */
    if (0 != strncmp("ping", input, 4)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? */
    if (strlen(input) < 6) {
        return ERROR_CMD_XBOARD_PING_MISSING_N;   
    }

    /* attempt to read the N parameter */
    int n;
    if (1 != sscanf(input + 5, "%d", &n)) {
        return ERROR_CMD_XBOARD_PING_MISSING_N;
    }

    /* wait for any active search to finish before responding */
    int retval = block_on_search_thread(false);
    if (retval != 0) {
        return retval;
    }

    plog("pong %d\n", n);
  
    return 0;
}
