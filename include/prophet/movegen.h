#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include <stdbool.h>

#include <prophet/position/position.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Given position \p p, is square \p sq attacked by \p player?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked(const position* p, square_t sq, color_t player);

/**
 * \brief Given position \p pos, is \p player in check?
 *
 * \param pos       The chess position
 * \param player    A player (white or black)
 *
 * \return - true if the player is in check, otherwise false.
 */
bool in_check(const position* pos, color_t player);


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
 * \param m         The start of a pre-allocated move stack.
 * \param p         The chess position
 * \param noncaps   If non-capturing moves should be generated
 * \param caps      If capturing moves should be generated
 *
 * \return - A move pointer one greater than the last move produced.
 */
move* gen_pseudo_legal_moves(move* m, const position* p, bool caps, bool noncaps);


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
 * \param m         The start of a pre-allocated move stack.
 * \param p         The chess position
 * \param noncaps   If non-capturing moves should be generated
 * \param caps      If capturing moves should be generated
 *
 * \return - A move pointer one greater than the last move produced.
 */
move* gen_legal_moves(move* m, const position* p, bool caps, bool noncaps);

/**
 * \brief Count all possible moves to a fixed depth.
 *
 * The term 'perft' was first introduced by Bob Hyatt of Cray Blitz and Crafty 
 * fame. It's basically a functional test that walks the move generation tree 
 * in depth first fashion, and returning a node (vertex) count when complete.  
 * Tested from a wide range of positions, this gives a very high level of 
 * confidence that the move generation and apply functions are working 
 * correctly.
 *
 * Another use of 'perft', (and perhaps Hyatt's intention given the name) is 
 * to measure the performance of the move gen/apply systems.  By recording the 
 * time it takes to complete a perft run and the node count it returns, we have
 * a metric -- nodes per second.
 *
 * Reference: http://chessprogramming.wikispaces.com/Perft
 *
 * \param pos       The starting chess position
 * \param depth     The depth of the tree from this position.
 *
 * \return - the number of moves generated
 */
uint64_t perft(position* pos, uint32_t depth);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _MOVEGEN_H_ */
