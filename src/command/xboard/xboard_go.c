#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/util/string_utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xboard_internal.h"

extern position gpos;
extern bool xboard_force_mode;

/**
 * \brief Execute the xboard go command 
 * 
 * Leave force mode and set the engine to play the color that is on move. 
 * Associate the engine's clock with the color that is on move, the opponent's 
 * clock with the color that is not on move. Start the engine's clock. Start 
 * thinking and eventually make a move.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_go(const char* input)
{
    /* verify the command */
    if (0 != strcmp("go", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    xboard_force_mode = false;

    /* start thinking and eventually make a move.  In this implementation we
     * move immediately. */
    move_t engine_mv = select_move(&gpos);
    undo_t u;
    apply_move(&gpos, engine_mv, &u);
    char* str_engine_mv = move_to_str(engine_mv);
    printf("move %s\n", str_engine_mv);
    free(str_engine_mv);

    return 0;
}

