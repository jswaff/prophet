#include <stdbool.h>
#include <string.h>

#include <prophet/command.h>
#include <prophet/error_codes.h>
#include <prophet/parameters.h>

bool xboard_force_mode;

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
int xboard_force(const char* input, int* exit_status)
{
    /* verify the command */
    if (0 != strcmp("force", input))
    {
        return P4_ERROR_CMD_INCORRECT_COMMAND;
    }

    /* TODO: stop search */
    
    xboard_force_mode = true;

    *exit_status = 0;

    return 0;
}

