#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/position/move.h>
#include <prophet/util/legal.h>
#include <prophet/util/select_move.h>
#include <prophet/util/string_utils.h>

extern position gpos;
extern bool xboard_force_mode;

/**
 * \brief Execute the xboard usermove command 
 * 
 * If the move is illegal, print an error message; see the section "Commands 
 * from the engine to xboard". If the move is legal and in turn, make it. If 
 * not in force mode, stop the opponent's clock, start the engine's clock, 
 * start thinking, and eventually make a move. 
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int xboard_usermove(const char* input, int* exit_status)
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
    move mv = str_to_move(str_mv, &gpos);
    if (NO_MOVE == mv)
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_INVALID_MOVE;
    }

    /* is the move legal? */
    if (!is_legal_move(mv, &gpos))
    {
        return P4_ERROR_CMD_XBOARD_USERMOVE_ILLEGAL_MOVE;
    }

    /* apply move */
    undo u;
    apply_move(&gpos, mv, &u);

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
    /* TODO: draw by lack of mating material */
    else if (!xboard_force_mode)
    {
        /* the game continues.  start thinking and (eventually) make a move. */
        move engine_mv = select_move(&gpos);
        apply_move(&gpos, engine_mv, &u);
        char* str_engine_mv = move_to_str(engine_mv);
        printf("move %s\n", str_engine_mv);
        free(str_engine_mv);
    }

    *exit_status = 0;

    return 0;
}
