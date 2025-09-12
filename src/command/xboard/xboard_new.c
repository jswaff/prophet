#include "xboard_internal.h"

#include "prophet/const.h"
#include "prophet/error_codes.h"
#include "prophet/hash.h"

#include "position/position.h"

#include <stdbool.h>
#include <string.h>

/* externs */
extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern bool xboard_force_mode;
extern int32_t volatile max_depth;
extern uint32_t volatile hash_age;
extern uint32_t num_random_starting_moves;
extern uint32_t random_moves_counter;


/**
 * @brief Execute the xboard new command 
 * 
 * Reset the board to the standard chess starting position. Set White on move. 
 * Leave force mode and set the engine to play Black. Associate the engine's 
 * clock with Black and the opponent's clock with White. Reset clocks and time 
 * controls to the start of a new game. Use wall clock for time measurement. 
 * Stop clocks. Do not ponder on this move, even if pondering is on. Remove any
 * search depth limit previously set by the sd command.  
 *
 * @param input         the user entered input 
 *
 * @return 0 on successful execution, and non-zero on failure
 */
int xboard_new(const char* input)
{
    /* verify the command */
    if (0 != strcmp("new", input)) {
        return ERROR_CMD_INCORRECT_COMMAND;
    }

    int retval = block_on_search_thread(true);
    if (0 != retval) {
        return retval;
    }

    reset_pos(&gpos);

    /* clear the undo information */
    memset(gundos, 0, sizeof(gundos));


    /* clear hash tables */
    clear_hash_tables();

    xboard_force_mode = false;

    random_moves_counter = num_random_starting_moves;
    max_depth = 0;
    hash_age = 0;

    return 0;
}
