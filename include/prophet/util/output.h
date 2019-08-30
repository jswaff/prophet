#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/**
 * \brief Write an error message to stderr.
 */
void error(const char* format, ...);


/**
 * \brief Write a error message to stdout.
 */
void out(FILE* stream, const char* format, ...);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _ERROR_H_ */
