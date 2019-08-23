#ifndef _SELECT_MOVE_H_
#define _SELECT_MOVE_H_

#include <prophet/position/move.h>
#include <prophet/position/position.h>


/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Select a move.
 *
 * Select a legal move from the given position.  If there are no legal moves,
 * NO_MOVE is returned.
 *
 * This is a temporary implementation that just chooses a move at random.
 *
 * \param pos           A pointer to the chess position.
 *
 * \return  a move to play, or NO_MOVE if there isn't one.
 */
move select_move(const position* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _SELECT_MOVE_H_ */
