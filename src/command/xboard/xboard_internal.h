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
 * \brief Execute the xboard level command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_level(const char* input);


/**
 * \brief Execute the xboard move now command
 * 
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_move_now(const char* input);


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
 * \brief Execute the xboard nopost command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_nopost(const char* input);


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
 * \brief Execute the xboard post command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_post(const char* input);


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
 * \brief Execute the xboard quit command
 * 
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_quit(const char* input);


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
 * \brief Execute the xboard sd command 
 * 
 * Respond to the sd DEPTH command by setting the maximum search depth.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_sd(const char* input);


/**
 * \brief Execute the xboard sd command 
 * 
 * Respond to the st TIME command by setting the maximum search time
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_st(const char* input);


/**
 * \brief Execute the xboard setboard command 
 * 
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_setboard(const char* input);


/**
 * \brief Execute the xboard time command 
 * 
 * Respond to the time command by setting the time remaining.  
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int xboard_time(const char* input);


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
 * \brief Select a move, and apply it to the game position.  If the game is
 * over, print the result.
 *
 * This method should not be called if the game is already over.
 *
 * This initial implementation is synchronous, but future implementations will
 * start a separate thread.
 */
int think_and_make_move();


/**
 * \brief Block on an active search thread.
 * 
 * Performs a join on any running search thread.  If the stop parameter
 * is true the search is forcefully stopped.
 *
 * \param stop          stop the search thread  
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int block_on_search_thread(bool stop);


/**
 * \brief Check for an end-of-game condition and print RESULT if appropriate.
 *
 * \return true if the game is over, false otherwise.
 */
bool endgame_check();


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _COMMAND_INTERNAL_H_ */
