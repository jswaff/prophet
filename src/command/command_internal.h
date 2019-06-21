#ifndef _COMMAND_INTERNAL_H_
#define _COMMAND_INTERNAL_H_

#include <prophet/command.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


int parse_and_execute(const char* input, int* exit_status);
int parse_command(user_command_t* user_cmd, const char* input);

int command_exit(const user_command_t*);
int command_no_op(const user_command_t*);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _COMMAND_INTERNAL_H_ */
