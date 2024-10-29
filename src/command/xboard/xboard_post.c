#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <string.h>

bool xboard_post_mode = true;

/**
 * \brief Execute the xboard post command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_post(const char* input)
{
    /* verify the command */
    if (0 != strncmp("post", input, 4)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_post_mode = true;

    /* success */
    return 0;
}
