#include <stdio.h>

#include <prophet/command.h>
#include <prophet/parameters.h>

int command_no_op(const user_command_t* UNUSED(user_command),
    int* exit_status)
{
    *exit_status = 0;
    
    printf("no op\n");
    return 0;
}

