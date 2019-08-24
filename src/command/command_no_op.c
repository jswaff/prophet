#include <prophet/command.h>
#include <prophet/parameters.h>

#include <stdio.h>

/**
 * \brief Execute the no op command - does nothing!
 * 
 * \param input         the user entered input (unused)
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_no_op(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 0;
    
    return 0;
}

