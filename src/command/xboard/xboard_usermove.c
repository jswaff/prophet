#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/movegen.h>
#include <prophet/util/string_utils.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xboard_internal.h"

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern bool xboard_force_mode;

/**
 * \brief Execute the xboard usermove command 
 * 
 * If the move is illegal, print an error message; see the section "Commands 
 * from the engine to xboard". If the move is legal and in turn, make it. If 
 * not in force mode, stop the opponent's clock, start the engine's clock, 
 * start thinking, and eventually make a move. 
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_usermove(const char* input)
{
    /* verify the command */
    if (0 != strncmp("usermove", input, 8))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* is the command long enough to contain an argument? 
     * the shortest possible valid command would hold a two character
     * move, e.g. "usermove e4"
     */
    if (strlen(input) < 11)
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_MISSING_MOVE;   
    }

    /* read the move */
    char str_mv[10];
    if (1 != sscanf(input + 9, "%s", str_mv))
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* parse the move */
    move_t mv = str_to_move(str_mv, &gpos);
    if (NO_MOVE == mv)
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* apply move */
    if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME)
    {
        return P4_ERROR_GUNDO_INDEX_UB_VIOLATION;
    }
    apply_move(&gpos, mv, gundos + gpos.move_counter);

    /* if the game is over by rule, print the result */
    if (is_checkmate(&gpos)) 
    {
        if (gpos.player == WHITE)
        {
            printf("0-1 {Black mates}\n");
        }
        else
        {
            printf("1-0 {White mates}\n");
        }
    }
    else if (is_stalemate(&gpos))
    {
        printf("1/2-1/2 {Stalemate}\n");
    }
    else if (is_lack_of_mating_material(&gpos))
    {
        printf("1/2-1/2 {Insufficient material}\n");
    }
    else if (is_draw50(&gpos))
    {
        printf("1/2-1/2 {50 move rule}\n");   
    }
    else if (is_draw_rep(&gpos, gundos))
    {
        printf("1/2-1/2 {Draw by repetition}\n");
    }
    else if (!xboard_force_mode)
    {
        /* the game continues.  start thinking and (eventually) make a move.
         * in this initial implementation we move immediately. */
        move_t engine_mv = select_move(&gpos);
        if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME)
        {
            return P4_ERROR_GUNDO_INDEX_UB_VIOLATION;
        }
        apply_move(&gpos, engine_mv, gundos + gpos.move_counter);
        char* str_engine_mv = move_to_str(engine_mv);
        printf("move %s\n", str_engine_mv);
        free(str_engine_mv);
    }

    return 0;
}
