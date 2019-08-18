#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <prophet/position/square.h>
#include <prophet/position/position.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Convert a string to a move.
 *
 * The string should have length four, except for caslting moves, which should
 * have a length of five.  SAN notation is not supported.  Proper formatting
 * is two characters for the source square, followed by two characters for the
 * destination square, followed by an optional promotion piece.
 * Examples: e2e4, e7e8q, e1g1
 *
 * \param str_mv   The string to convert.
 * 
 * \return  The move if valid, or BADMOVE on failure.
 */
move str_to_move(const char* str_mv);

/**
 * \brief Convert a string to a square.
 *
 * The string should have a length of two.  The first character should be a 
 * letter A-H (lower or upper),and the second a number 1-8.  E.g. A1, b2, e3 
 * are all valid.
 *
 * \param str_sq   The string to convert.
 *
 * \return  The square if valid, or NO_SQUARE on failure.
 */
square_t str_to_sq(const char* str_sq);

/**
 * \brief Convert a square to a string.
 *
 * Returns a pointer to a null-terminated byte string.  The returned  pointer
 * must be passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param sq        The square to convert
 *
 * \return A null terminated byte string.
 */ 
char* sq_to_str(square_t sq);

/**
 * \brief Convert a position into a string.
 *
 * Returns a pointer to a null-terminated byte string.  The returned pointer
 * must be passed to free to avoid a memory leak.
 *
 * If an error occurs, a null pointer is returned.
 *
 * \param pos       A pointer to a chess position.
 *
 * \return A null terminated byte string.
 */
char* pos_to_str(const position* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _SQUARE_H_ */
