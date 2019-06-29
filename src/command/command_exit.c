#include <stdio.h>

#include <prophet/command.h>
#include <prophet/parameters.h>

/**
 * \brief Execute the exit command.
 * 
 * \param input         The user entered input (unused)
 * \param exit_status   Pointer to receive exit status.  A non-zero 
 *                      status indicates the program should exit.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int command_exit(const char* UNUSED(input), int* exit_status)
{
    *exit_status = 1;

    printf("bye.\n");

    return 0;
}

