#include "xboard_internal.h"

#include "prophet/error_codes.h"

#include <stdbool.h>
#include <string.h>

bool xboard_force_mode;


/**
 * \brief Execute the xboard force command 
 * 
 * Set the engine to play neither color ("force mode"). Stop clocks. The engine
 * should check that moves received in force mode are legal and made in the 
 * proper turn, but should not think, ponder, or make moves of its own.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_force(const char* input)
{
    /* verify the command */
    if (0 != strcmp("force", input)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = true;
    int retval = block_on_search_thread(true);

    return retval;
}

