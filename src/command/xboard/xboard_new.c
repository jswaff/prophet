#include <stdio.h>

#include <prophet/position/position.h>
#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

extern position gpos;

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
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_new(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 0;

    reset_pos(&gpos);

    return 0;
}

