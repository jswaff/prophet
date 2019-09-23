#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/position/position.h>

#include <string.h>

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

    /* create a copy of the position that we can modify */
    position_t copy_pos;
    memcpy(&copy_pos, pos, sizeof(position_t));

    /* iterate over the move list, looking for the one with the highest
     * static evaluation */
    move_t best_mv = NO_MOVE;
    int32_t best_score = 0;
    undo_t u;

    for (move_t* mp = moves; mp < endp; mp++) 
    {
        if (*mp != NO_MOVE)
        {
            apply_move(&copy_pos, *mp, &u);
            int32_t score = -eval(&copy_pos);
            undo_move(&copy_pos, &u);

            if (best_mv == NO_MOVE || score > best_score)
            {
                best_mv = *mp;
                best_score = score;
            }
        }
    }

    /* return the best move */
    return best_mv;
}
