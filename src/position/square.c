#include "prophet/square.h"

#include "square_internal.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static dir_t direction_tbl[64][64];

static dir_t get_dir_slow(square_t from, square_t to);


rank_t get_rank(square_t sq)
{
    assert(sq >= A8 && sq <= H1);
    return (rank_t)(sq >> 3);
}


file_t get_file(square_t sq)
{
    assert(sq >= A8 && sq <= H1);
    return (file_t)(sq & 7);
}


square_t get_square(rank_t rank, file_t file)
{
    assert(rank >= RANK_8 && rank <= RANK_1);
    assert(file >= FILE_A && file <= FILE_H);
    return (square_t)(rank*8+file);
}


uint64_t squares_to_bitmap(square_t sq, ...)
{
    uint64_t val = 0;

    va_list sqs;
    va_start(sqs, sq);

    while (sq >= 0 && sq < 64) {
        val |= (uint64_t)1 << sq;
        sq = va_arg(sqs, int);
    }

    va_end(sqs);

    return val;
}


dir_t get_dir(square_t from, square_t to)
{
    assert(from >= A8 && from <= H1);
    assert(to >= A8 && to <= H1);

    return direction_tbl[from][to];
}


uint32_t rank_distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);
    return abs((int)get_rank(sq1)-(int)get_rank(sq2));
}


uint32_t file_distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);
    return abs((int)get_file(sq1)-(int)get_file(sq2));
}


uint32_t distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);

    int32_t rd = rank_distance(sq1, sq2);
    int32_t fd = file_distance(sq1, sq2);

    return rd > fd ? rd : fd;
}


square_t north(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8) return NO_SQUARE;
    return (square_t)(sq - 8);
}


square_t north2(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) <= RANK_7) return NO_SQUARE;
    return (square_t)(sq - 16);
}


square_t south(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1) return NO_SQUARE;
    return (square_t)(sq + 8);
}


square_t south2(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) >= RANK_2) return NO_SQUARE;
    return (square_t)(sq + 16);
}


square_t east(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_file(sq) == FILE_H) return NO_SQUARE;
    return (square_t)(sq + 1);
}


square_t west(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_file(sq) == FILE_A) return NO_SQUARE;
    return (square_t)(sq - 1);
}


square_t northeast(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8 || get_file(sq) == FILE_H) 
        return NO_SQUARE;
    return (square_t)(sq - 7);
}


square_t southeast(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1 || get_file(sq) == FILE_H) 
        return NO_SQUARE;
    return (square_t)(sq + 9);
}


square_t southwest(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1 || get_file(sq) == FILE_A) 
        return NO_SQUARE;
    return (square_t)(sq + 7);
}


square_t northwest(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8 || get_file(sq) == FILE_A) 
        return NO_SQUARE;
    return (square_t)(sq - 9);
}


bool is_light_sq(square_t sq)
{
    assert(sq>=A8 && sq<=H1);
    rank_t r = get_rank(sq);
    file_t f = get_file(sq);
    return (r%2) == (f%2);
}


bool is_dark_sq(square_t sq)
{
    assert(sq>=A8 && sq<=H1);
    return !is_light_sq(sq);
}


bool is_diagonal(square_t sq1, square_t sq2)
{
    return rank_distance(sq1,sq2)==file_distance(sq1,sq2);
}


static dir_t get_dir_slow(square_t from, square_t to)
{
    int32_t f_diff = get_file(from) - get_file(to);
    int32_t r_diff = get_rank(from) - get_rank(to);

    if (f_diff == 0) {
        /* same file */
        if (r_diff < 0) return SOUTH;
        if (r_diff > 0) return NORTH;
    } else if (f_diff < 0) {
        /* from is west of to */
        if (r_diff == 0) return EAST;
        if (r_diff == f_diff) return SOUTHEAST;
        if (r_diff == -f_diff) return NORTHEAST;
    } else { 
        /* f_diff > 0 */
        /* from is east of to */
        if (r_diff == 0) return WEST;
        if (r_diff == f_diff) return NORTHWEST;
        if (r_diff == -f_diff) return SOUTHWEST;
    }

    return NODIR;
}


void init_squares()
{
    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            direction_tbl[i][j] = get_dir_slow((square_t)i,(square_t)j);
        }
    }
}
