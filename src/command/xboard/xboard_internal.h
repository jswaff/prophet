#ifndef _XBOARD_INTERNAL_H_
#define _XBOARD_INTERNAL_H_

#include <prophet/command.h>

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
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_force(const char* input, int* exit_status);

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
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_new(const char* input, int* exit_status);


/**
 * \brief Execute the xboard ping command 
 * 
 * Respond to the ping N command with a pong N.  
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_ping(const char* input, int* exit_status);

/**
 * \brief Execute the xboard protover command 
 * 
 * Respond to the protover command by printing a list of supported features.  
 *
 * \param input         The user entered input 
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int xboard_protover(const char* input, int* exit_status);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _COMMAND_INTERNAL_H_ */
