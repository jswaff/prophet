#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_


// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


/**
 * \brief Parse command line options.
 * 
 * \param argc          Number of arguments.
 * \param argv          List of arguments.
 *
 * \returns 0 on successful execution, and non-zero on failure.
 */
int process_commandline_options(int argc, const char** argv);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _COMMANDLINE_H_ */
