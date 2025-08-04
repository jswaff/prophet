#pragma once

#include <prophet/move.h>
#include <prophet/position.h>

#include <stdbool.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Generate a list of pseudo-legal moves.
 *
 * It is guaranteed that all legal moves are generated.  However, no 
 * verification is done to determine if a move would leave the current 
 * player's king in check.
 *
 * It is assumed that the move stack contains enough storage for all moves 
 * generated.
 *
 * \param m             the start of a pre-allocated move stack
 * \param pos           a pointer to a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
move_t* gen_pseudo_legal_moves(move_t *m, const position_t *pos, bool caps, bool noncaps);


/**
 * \brief Generate a list of legal moves.
 *
 * A complete list of strictly legal moves.
 *
 * It is assumed that the move stack contains enough storage for all 
 * (pseudo-legal) moves generated.  The move list will contain NO_MOVE for 
 * pseudo-legal moves that were analyzed and found to be illegal.
 *
 * Note this method is significantly slower than generating pseudo-legal moves!
 *
 * \param m             the start of a pre-allocated move stack.
 * \param pos           a pointer to a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
move_t* gen_legal_moves(move_t *m, const position_t *pos, bool caps, bool noncaps);


/**
 * \brief Test move legality.
 *
 * Test that a move is legal in a given position.
 *
 * \param mv            the move to test
 * \param pos           a pointer to a chess position
 *
 * \return true if legal, otherwise false
 */
bool is_legal_move(move_t mv, const position_t *pos);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

