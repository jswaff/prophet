#include <stdio.h>

#include <prophet/command.h>
#include <prophet/parameters.h>

/**
 * \brief Default command handler - does nothing!
 * 
 * \param input         The user entered input (unused)
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int command_no_op(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 0;
    
    printf("unknown command\n");
    
    return 0;
}

