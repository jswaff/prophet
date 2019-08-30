#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/movegen.h>
#include <prophet/util/output.h>
#include <prophet/util/string_utils.h>

#include <assert.h>
#include <stdlib.h>

#include "xboard_internal.h"

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];


/**
 * \brief Select a move, and apply it to the game position.  If the game is
 * over, print the result.
 *
 * This method should not be called if the game is already over.
 *
 * This initial implementation is synchronous, but future implementations will
 * start a separate thread.
 */
int think_and_make_move()
{
    assert(!endgame_check());

    move_t engine_mv = select_move(&gpos);
    if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME)
    {
        return P4_ERROR_GUNDO_INDEX_UB_VIOLATION;
    }
    apply_move(&gpos, engine_mv, gundos + gpos.move_counter);
    char* str_engine_mv = move_to_str(engine_mv);
    out(stdout, "move %s\n", str_engine_mv);
    free(str_engine_mv);

    endgame_check();

    return 0;
}
