#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <prophet/position/square.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Convert a string to a square.
 *
 * The string should have a length of two.  The first character should be a letter A-H (lower or upper),
 * and the second a number 1-8.  E.g. A1, b2, e3 are all valid.
 *
 * \param str_sq   The string to convert.
 *
 * \return  The square if valid, or NO_SQUARE on failure.
 */
square_t str_to_sq(const char *str_sq);

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SQUARE_H_ */
