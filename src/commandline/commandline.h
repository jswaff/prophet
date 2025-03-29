#pragma once

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


/**
 * \brief Load neural network weights file
 * 
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_load_network(const char* weights_file);


/**
 * \brief Load eval properties
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_load_properties(const char* props_file);


/**
 * \brief Print command line options.
 */
void commandline_print_usage();


/**
 * \brief Parse command line options.
 * 
 * \param argc          number of arguments
 * \param argv          list of arguments
 *
 * \return 0 on successful execution, and non-zero on failure
 */
int commandline_process_options(int argc, char* argv[]);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
