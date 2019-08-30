#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_


/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Parse command line options.
 * 
 * \param argc          number of arguments
 * \param argv          list of arguments
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int process_commandline_options(int argc, const char** argv);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _COMMANDLINE_H_ */
