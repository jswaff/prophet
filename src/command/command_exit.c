#include <stdio.h>

#include <prophet/command.h>
#include <prophet/parameters.h>

int command_exit(const user_command_t* UNUSED(user_command),
    int* exit_status)
{
    *exit_status = 1;
    printf("bye.\n");

    return 0;
}

