#ifndef _COMMAND_H_
#define _COMMAND_H_


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

typedef struct user_command user_command_t;

typedef int (*cmd_func_t)(const user_command_t*, int* exit_status);

struct user_command
{
    char* base_cmd;
    cmd_func_t cmd_func;
};


int command_loop();


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _COMMAND_H_ */
