#ifndef _ERROR_H_
#define _ERROR_H_

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Write an error message to stderr.
 *
 */
void error(const char* format, ...);

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _ERROR_H_ */
