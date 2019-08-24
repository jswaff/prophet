#ifndef _MOVEGEN_H_
#define _MOVEGEN_H_

#include <prophet/position/position.h>

#include <stdbool.h>
#include <stdint.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Given position \p pos, is square \p sq attacked by \p player?
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked(const position* pos, square_t sq, color_t player);


/**
 * \brief Given position \p pos, is \p player in check?
 *
 * \param pos           a pointer to a chess position
 * \param player        a player (white or black)
 *
 * \return true if the player is in check, otherwise false
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
 * \param m             the start of a pre-allocated move stack
 * \param pos           a pointer to a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
move_t* gen_pseudo_legal_moves(
    move_t* m, const position* pos, bool caps, bool noncaps);


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
move_t* gen_legal_moves(
    move_t* m, const position* pos, bool caps, bool noncaps);


/**
 * \brief Count the number of legal moves possible in a position.
 *
 * \param pos           a pointer to a chess position
 * \param caps          whether to include captures in the count
 * \param noncaps       whether to include noncaptures in the count
 *
 * \return the number of legal moves
 */
uint32_t num_legal_moves(const position* pos, bool caps, bool noncaps);


/**
 * \brief Count the number of capture and non-capture moves in a list.
 *
 * The memory range is iterated, beginning with \p startp and ending with 
 * \p endp - 1. Some slots may contain an invalid move (NO_MOVE).  These 
 * "moves" are not counted.
 *
 * \param startp        the starting address of a list of moves (inclusive)
 * \param endp          the ending address of a list of moves (exclusive)
 * \param caps          a pointer to an integer to receive the number of 
 *                      captures
 * \param noncaps       a pointer to an integer to receive the number of 
 *                      noncaptures
 */
void num_moves_in_list(
    const move_t* startp, const move_t* endp, int* caps, int* noncaps);


/**
 * \brief Has the current player been checkmated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been checkmated, otherwise false
 */
bool is_checkmate(const position* pos);


/**
 * \brief Has the current player been stalemated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been stalemated, otherwise false
 */
bool is_stalemate(const position* pos);


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
bool is_legal_move(move_t mv, const position* pos);


/**
 * \brief Test if a move is a member of a list.
 *
 * Determine if a chess move is contained within a list of moves. The score 
 * portion of the move is ignored.
 *
 * \param mv            the move to look for
 * \param start         a pointer to the start of a move list
 * \param end           a pointer one past the end of a move list
 *
 * \return true if the move is contained in the list, otherwise false
 */
bool is_in_move_list(move_t mv, const move_t* start, const move_t* end);


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
 * \param pos           a pointer to a chess position
 * \param depth         the depth of the tree from this position
 *
 * \return the number of moves generated
 */
uint64_t perft(position* pos, uint32_t depth);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _MOVEGEN_H_ */
