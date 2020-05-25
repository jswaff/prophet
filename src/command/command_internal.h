#ifndef _COMMAND_INTERNAL_H_
#define _COMMAND_INTERNAL_H_

#include <prophet/command.h>

#include <stdbool.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Parse the user input into a command and execute the command.
 *
 * \param input         the user entered input
 * \param exit_status   pointer to boolean to receive exit status
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int parse_and_execute(const char* input, bool* exit_status);


/**
 * \brief Parse a command
 * 
 * Parse the user input and attempt to map it to a command.  Note that failure
 * to map the input to a command does not generate a failure return code.  In 
 * this case the input is mapped to the no-op command.
 *
 * Implementation note: for the most part the commands specific to XBoard are 
 * isolated in the xboard folder.  Some of the XBoard logic has leaked up, but
 * it's pretty minimal and wouldn't be hard to refactor if support for another
 * protocol were added in the future.
 *
 * \param cmd           pointer to structure to receive parsed command
 * \param input         buffer containing command to be parsed
 * \param exit_status   pointer to boolean to receive exit status
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int parse_command(
    user_command_t* user_cmd, const char* input, bool* exit_status);


/**
 * \brief Execute the db (drawboard) command.
 *
 * Draw the board to stdout.
 *
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_db(const char* input);


/**
 * \brief Execute the eval command.
 * 
 * Evaluate the current position and print the score.
 *
 * \param input         the user entered input 
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_eval(const char* input);


/**
 * \brief Default command handler - does nothing!
 * 
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_no_op(const char* input);


/**
 * \brief Execute the perft command.
 * 
 * Command should be in the format "perft D", where D is depth.  The perft is
 * executed iteratively, starting with depth=1 and ending with depth=D.
 * The number of nodes and nodes-per-second are printed to stdout for each
 * iteration.
 *
 * \param input         the user entered input
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_perft(const char* input);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _COMMAND_INTERNAL_H_ */
