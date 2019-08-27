#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/position/position.h>

#include <string.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern bool xboard_force_mode;

/**
 * \brief Execute the xboard new command 
 * 
 * Reset the board to the standard chess starting position. Set White on move. 
 * Leave force mode and set the engine to play Black. Associate the engine's 
 * clock with Black and the opponent's clock with White. Reset clocks and time 
 * controls to the start of a new game. Use wall clock for time measurement. 
 * Stop clocks. Do not ponder on this move, even if pondering is on. Remove any
 * search depth limit previously set by the sd command.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_new(const char* input)
{
    /* verify the command */
    if (0 != strcmp("new", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    reset_pos(&gpos);

    /* clear the undo information */
    memset(gundos, 0, sizeof(gundos));

    xboard_force_mode = false;

    return 0;
}
