#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/position/position.h>
#include <prophet/search.h>
#include <prophet/util/output.h>
#include <prophet/util/string_utils.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];

bool random_mode = false;

/* move stack */
move_t moves[MAX_PLY * MAX_MOVES_PER_PLY];

/* forward decls */
static move_t select_random_move(
    const move_t* startp, const move_t* endp, int num_moves);

/**
 * \brief Select a move.
 *
 * Select a legal move from the given position.  If there are no legal moves,
 * NO_MOVE is returned.
 *
 * \param pos           a pointer to the chess positions
 *
 * \return a move to play, or NO_MOVE if there isn't one
 */
move_t select_move(const position_t* pos)
{
    /* generate legal moves */
    move_t moves[MAX_PLY * MAX_MOVES_PER_PLY]; /* TODO: move this */
    move_t *endp = gen_legal_moves(moves, pos, true, true);

    /* count the number of moves to choose from */
    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);
    int num_moves = num_caps + num_noncaps;

    /* if there aren't any moves to choose from, just return NO_MOVE */
    if (num_moves == 0)
    {
        return NO_MOVE;
    }

    /* if we're in random mode, just pick a move */
    if (random_mode)
    {
        return select_random_move(moves, endp, num_moves);
    }

    /* create a copy of the position that we can modify */
    position_t copy_pos;
    memcpy(&copy_pos, pos, sizeof(position_t));

    /* create an undo stack and initialize - TODO: move this */
    undo_t undos[MAX_PLY];
    memcpy(undos, gundos, pos->move_counter * sizeof(undo_t));

    /* search the position to a fixed depth */
    move_line_t pv = iterate(&copy_pos, false, moves, undos);


    /* return the best move */
    assert(pv.n > 0);

    return pv.mv[0];
}


/**
 * \brief Choose a move randomly.
 *
 * \param startp        the starting address of a list of moves (inclusive)
 * \param endp          the ending address of a list of moves (exclusive)
 * \param num_moves     the number of valid moves in the list.
 *
 * \return a move.
 */
static move_t select_random_move(
    const move_t* startp, const move_t* endp, int num_moves)
{
    /* choose a random move. */
    int mv_ind = rand() % num_moves;

    /* fetch the move from the stack.  since the list isn't contiguous, (it
     * contains some NO_MOVE entries), so we need to iterate. */
    int i = 0;
    for (const move_t* mp = startp; mp < endp; mp++) 
    {
        if (*mp != NO_MOVE)
        {
            if (i == mv_ind)
            {
                return *mp;
            }
            i++;
        }
    }

    /* we should never get here, but the compiler doesn't know that. */
    assert(false);
    return NO_MOVE;
}
