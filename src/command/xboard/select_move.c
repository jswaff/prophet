#include <prophet/const.h>
#include <prophet/movegen.h>
#include <prophet/position/position.h>

#include <stdlib.h>


/**
 * \brief Select a move.
 *
 * Select a legal move from the given position.  If there are no legal moves,
 * NO_MOVE is returned.
 *
 * This is a temporary implementation that just chooses a move at random.
 *
 * \param pos           a pointer to the chess positions
 *
 * \return a move to play, or NO_MOVE if there isn't one
 */
move_t select_move(const position* pos)
{
    /* generate legal moves */
    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_legal_moves(moves, pos, true, true);

    /* count the number of moves to choose from */
    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);
    int num_moves = num_caps + num_noncaps;

    if (num_moves == 0)
    {
        return NO_MOVE;
    }

    /* choose a random move.  system rand isn't so great but this is temporary
     * anyway. */
    int mv_ind = rand() % num_moves;

    /* fetch the move from the stack.  since the list isn't contiguous, (it
     * contains some NO_MOVE entries), we need to iterate. */
    int i = 0;
    for (move_t* mp = moves; mp < endp; mp++) 
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

    /* we should never get here. */
    return NO_MOVE;
}
