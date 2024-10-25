#include "xboard_internal.h"

#include "prophet/error_codes.h"
#include "prophet/position/position.h"

#include "../../util/output.h"

#include <string.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];

/**
 * \brief Execute the xboard setboard command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_setboard(const char* input)
{
    /* verify the command */
    if (0 != strncmp("setboard", input, 8)) {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain a valid FEN? The shortest possible
     * valid FEN would be a board with two kings, e.g. k7/8/8/8/8/8/8/K8 w - - 
     * (leaving off the half move clock and full move number), so min 23 characters
     * in addition to the command itself.
     */
    if (strlen(input) < 32) {
        return P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_FEN;   
    }

    /* set the board */
    if (!set_pos(&gpos, input + 9)) {
        plog("tellusererror Illegal position\n");
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_FEN;
    }

    /* clear the undo information */
    memset(gundos, 0, sizeof(gundos));

    /* success */
    return 0;
}
