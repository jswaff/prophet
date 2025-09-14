#include "xboard_internal.h"

#include "prophet/const.h"
#include "prophet/error_codes.h"

#include "position/position.h"

#include <string.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];


int xboard_remove(const char *input)
{
    /* verify the command */
    if (0 != strcmp("remove", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    if (gpos.move_counter < 2) {
        return ERROR_GUNDO_INDEX_LB_VIOLATION;
    }

    undo_move(&gpos, gundos + gpos.move_counter - 1);
    undo_move(&gpos, gundos + gpos.move_counter - 1);

    return 0;
}
