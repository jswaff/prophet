#ifndef _XBOARD_INTERNAL_H_
#define _XBOARD_INTERNAL_H_

#include <prophet/position/position.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   


/**
 * \brief Execute the xboard force command 
 * 
 * Set the engine to play neither color ("force mode"). Stop clocks. The engine
 * should check that moves received in force mode are legal and made in the 
 * proper turn, but should not think, ponder, or make moves of its own.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_force(const char* input);


/**
 * \brief Execute the xboard go command 
 * 
 * Leave force mode and set the engine to play the color that is on move. 
 * Associate the engine's clock with the color that is on move, the opponent's 
 * clock with the color that is not on move. Start the engine's clock. Start 
 * thinking and eventually make a move.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_go(const char* input);


/**
 * \brief Execute the xboard new command 
 * 
 * Reset the board to the standard chess starting position. Set White on move. 
 * Leave force mode and set the engine to play Black. Associate the engine's 
 * clock with Black and the opponent's clock with White. Reset clocks and time 
 * controls to the start of a new game. Use wall clock for time measurement. 
 * Stop clocks. Do not ponder on this move, even if pondering is on. Remove any
 * search depth limit previously set by the sd command.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_new(const char* input);


/**
 * \brief Execute the xboard ping command 
 * 
 * Respond to the ping N command with a pong N.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_ping(const char* input);


/**
 * \brief Execute the xboard protover command 
 * 
 * Respond to the protover command by printing a list of supported features.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_protover(const char* input);


/**
 * \brief Execute the xboard remove command 
 * 
 * Undo the previous FULL move - one move for each player.  XBoard protocol 
 * guarantees that this command will only be sent when the user is on move.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_remove(const char* input);


/**
 * \brief Execute the xboard result command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_result(const char* input);


/**
 * \brief Execute the xboard setboard command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_setboard(const char* input);


/**
 * \brief Execute the xboard undo command 
 * 
 * Undo the previous move.  XBoard protocol guarantees that the engine will be
 * in force mode before invoking this command.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_undo(const char* input);


/**
 * \brief Execute the xboard usermove command 
 * 
 * If the move is illegal, print an error message; see the section "Commands 
 * from the engine to xboard". If the move is legal and in turn, make it. If 
 * not in force mode, stop the opponent's clock, start the engine's clock, 
 * start thinking, and eventually make a move. 
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_usermove(const char* input);


/**
 * \brief Select a move.
 *
 * Select a legal move from the given position.  If there are no legal moves,
 * NO_MOVE is returned.
 *
 * This is a temporary implementation that just chooses a move at random.
 *
 * \param pos           a pointer to the chess position
 *
 * \return a move to play, or NO_MOVE if there isn't one
 */
move_t select_move(const position_t* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _COMMAND_INTERNAL_H_ */
