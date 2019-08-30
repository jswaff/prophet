#ifndef _COMMAND_H_
#define _COMMAND_H_


/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef struct user_command user_command_t;

typedef int (*cmd_func_t)(const char*);

struct user_command
{
    char* cmd;
    cmd_func_t cmd_func;
};

/**
 * \brief Execute the command loop.
 *
 * Repeatedly reads, parses, and executes user commands until a command sets
 * the exit status flag.
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int command_loop();


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif   

#endif /* ! defined _COMMAND_H_ */
