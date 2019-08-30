#ifndef _ERROR_H_
#define _ERROR_H_

#include <stdio.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/**
 * \brief Write an error message to stderr.
 *
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void error(const char* format, ...);


/**
 * \brief Write a message to a stream.
 *
 * \param stream        the stream to write to
 * \param format        formatted string, followed by variable length
 *                      list of arguments.
 */
void out(FILE* stream, const char* format, ...);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _ERROR_H_ */
