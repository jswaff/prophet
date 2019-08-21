#ifndef _LEGAL_H_
#define _LEGAL_H_

#include <prophet/position/move.h>
#include <prophet/position/position.h>


/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Test move legality.
 *
 * Test that a move is legal in a given position.
 *
 * \param mv            The move to test.
 * \param pos           A pointer to the chess position.
 *
 * \returns true if legal, otherwise false.
 */
bool is_legal_move(move mv, const position* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _LEGAL_H_ */
