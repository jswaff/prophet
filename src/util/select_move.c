#include <stdbool.h>

#include <prophet/position/move.h>
#include <prophet/position/position.h>
#include <prophet/const.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>


/**
 * \brief Select a move.
 *
 * Select a legal move from the given position.  If there are no legal moves,
 * NO_MOVE is returned.
 *
 * \param pos           A pointer to the chess position.
 *
 * \returns a move to play, or NO_MOVE if there isn't one.
 */
move select_move(const position* UNUSED(pos))
{
    //move moves[MAX_MOVES_PER_PLY];
    //move *endp = gen_legal_moves(moves, pos, true, true);

    return NO_MOVE;
}
