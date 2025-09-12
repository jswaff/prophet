#include "xboard_internal.h"

#include "prophet/const.h"
#include "prophet/error_codes.h"

#include "position/position.h"
#include "util/string_utils.h"

#include <stdio.h>
#include <string.h>

/* externs */
extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern bool xboard_force_mode;


int xboard_move(const char *input)
{
    /* verify the command */
    if ((strlen(input) < 4) || (strlen(input) > 5)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }
    
    /* read the move */
    char str_mv[10];
    if (1 != sscanf(input, "%s", str_mv)) {
        return ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    } 

    /* parse the move */
    move_t mv = str_to_move(str_mv, &gpos);
    if (NO_MOVE == mv) {
        return ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* apply move */
    if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME) {
        return ERROR_GUNDO_INDEX_UB_VIOLATION;
    }
    apply_move(&gpos, mv, gundos + gpos.move_counter);


    if (!endgame_check() && !xboard_force_mode) {
        return think_and_make_move();
    }

    return 0;
}
