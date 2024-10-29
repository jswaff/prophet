#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <string.h>


/**
 * \brief Execute the xboard move now command
 * 
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_move_now(const char* input)
{
    /* verify the command */
    if (0 != strcmp("?", input)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    return block_on_search_thread(true);
}
