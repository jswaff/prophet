#pragma once

#include <prophet/move.h>

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
 * \param n             the method will record the number of elements used in the stack
 * \param fen           a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 */
void generate_moves_from_fen(move_t* m, int* n, const char* fen, bool caps, bool noncaps);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

