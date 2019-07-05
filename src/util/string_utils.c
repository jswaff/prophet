#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <prophet/util/string_utils.h>
#include <prophet/const.h>
#include <prophet/parameters.h>

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
square_t str_to_sq(const char *str_sq)
{
    if (strlen(str_sq) != 2) {
        return NO_SQUARE;
    }
    int sq;

    // the first character should be a-h or A-H
    if (str_sq[0] >= 'a' && str_sq[0] <= 'h') {
        sq = str_sq[0] - 'a';
    } else if (str_sq[0] >= 'A' && str_sq[0] <= 'H') {
        sq = str_sq[0] - 'A';
    } else {
        return NO_SQUARE;
    }

    // the second character should be a digit 1-8
    if (str_sq[1] >= '1' && str_sq[1] <= '8') {
        sq += 8*(8-(str_sq[1]-'0'));
    } else {
        return NO_SQUARE;
    }

    return (square_t)sq;
}

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
char* sq_to_str(square_t sq)
{
    if (sq < 0 || sq > 63)
        return NULL;
    
    char* buf = (char*)malloc(3 * sizeof(char));

    buf[0] = 'a' + get_file(sq);
    buf[1] = '0' + (8 - get_rank(sq));
    buf[2] = 0;

    return buf;
}
