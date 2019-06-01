#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>

#include <prophet/position/square.h>
#include <prophet/parameters.h>

static uint64_t bb_files[8];
static uint64_t bb_ranks[8];
static dir_t direction_tbl[64][64];

static dir_t get_dir_slow(square_t from, square_t to);

/**
 * \brief Create a bitmap representation of a list of squares.
 *
 * Square A8 is the LSB, and square H1 the MSB
 *
 * \param  sq   A list of squares.  Should be terminated with NO_SQUARE.
 *
 * \return  A bitmap (bitboard) representation of the squares.
 */
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

/**
 * \brief Create a bitmap representation of a file (8 squares).
 *
 * FILE_A represents the 8 least significant bits.  FILE_H represents the
 * 8 most significant bits.
 *
 * \param f     A file
 *
 * \return  A bitmap representation of the file.
 */
uint64_t file_to_bitmap(file_t f)
{
    assert(f >= FILE_A && f <= FILE_H);

    return bb_files[f];
}

/**
 * \brief Create a bitmap representation of a rank (8 squares).
 *
 * \param f     A rank
 *
 * \return  A bitmap representation of the rank.
 */
uint64_t rank_to_bitmap(rank_t r)
{
    assert(r >= RANK_8 && r <= RANK_1);

    return bb_ranks[r];
}

/**
 * \brief Get the direction from one square to another.
 *
 * One of the eight compass directions, NORTH, NORTHEAST, etc will be returned,
 * or NODIR if the two squares do not occupy the same rank, file, or diagonal.
 * The black side of the board is considered "north".  So,
 *   get_dir(E4, E5) ==> NORTH
 *   get_dir(H8, A1) ==> SOUTHWEST
 *   get_dir(D7, E5) ==> NODIR
 *
 * \param from      The from square
 * \param to        The to square
 *
 * \return The direction of travel
 */
dir_t get_dir(square_t from, square_t to)
{
    assert(from >= A8 && from <= H1);
    assert(to >= A8 && to <= H1);

    return direction_tbl[from][to];
}

/**
 * \brief Get the rank a square occupies.
 *
 * \param sq        A square
 *
 * \return The rank the square occupies.
 */
rank_t get_rank(square_t sq)
{
    assert(sq >= A8 && sq <= H1);
    return (rank_t)(sq >> 3);
}


/**
 * \brief Get the file a square occupies.
 *
 * \param sq        A square
 *
 * \return The file the square occupies.
 */
file_t get_file(square_t sq)
{
    assert(sq >= A8 && sq <= H1);
    return (file_t)(sq & 7);
}

/**
 * \brief Get the square that occupies a given rank and file.
 *
 * \param rank      A rank
 * \param file      A file
 *
 * \return The square
 */
square_t get_square(rank_t rank, file_t file)
{
    assert(rank >= RANK_8 && rank <= RANK_1);
    assert(file >= FILE_A && file <= FILE_H);
    return (square_t)(rank*8+file);
}

/**
 * \brief Get the rank distance between two squares.
 *
 * Distance is returned as an absolute value.
 *
 * \param sq1       Square 1
 * \param sq2       Square 2
 *
 * \return  integer 0-7 representing the distance
 */
uint32_t rank_distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);
    return abs((int)get_rank(sq1)-(int)get_rank(sq2));
}

/**
 * \brief Get the files distance between two squares.
 *
 * Distance is returned as an absolute value.
 *
 * \param sq1       Square 1
 * \param sq2       Square 2
 *
 * \return  integer 0-7 representing the distance
 */
uint32_t file_distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);
    return abs((int)get_file(sq1)-(int)get_file(sq2));
}

/**
 * \brief Get the distance between two squares.
 *
 * Distance is defined as the greater of rank distance and file distance.
 *
 * \param sq1       Square 1
 * \param sq2       Square 2
 *
 * \return  integer 0-7 representing the distance
 */
uint32_t distance(square_t sq1, square_t sq2)
{
    assert(sq1 >= A8 && sq1 <= H1);
    assert(sq2 >= A8 && sq2 <= H1);

    int32_t rd = rank_distance(sq1, sq2);
    int32_t fd = file_distance(sq1, sq2);

    return rd > fd ? rd : fd;
}

/**
 * \brief Get the next square to the north
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t north(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8) return NO_SQUARE;
    return (square_t)(sq - 8);
}

/**
 * \brief Get the next square to the south
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t south(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1) return NO_SQUARE;
    return (square_t)(sq + 8);
}

/**
 * \brief Get the next square to the east
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t east(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_file(sq) == FILE_H) return NO_SQUARE;
    return (square_t)(sq + 1);
}

/**
 * \brief Get the next square to the west
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t west(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_file(sq) == FILE_A) return NO_SQUARE;
    return (square_t)(sq - 1);
}

/**
 * \brief Get the next square to the northeast
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t northeast(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8 || get_file(sq) == FILE_H) return NO_SQUARE;
    return (square_t)(sq - 7);
}

/**
 * \brief Get the next square to the southeast
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t southeast(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1 || get_file(sq) == FILE_H) return NO_SQUARE;
    return (square_t)(sq + 9);
}

/**
 * \brief Get the next square to the southwest
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t southwest(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_1 || get_file(sq) == FILE_A) return NO_SQUARE;
    return (square_t)(sq + 7);
}

/**
 * \brief Get the next square to the northwest
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * \param sq        A square
 *
 * \return the next square
 */
square_t northwest(square_t sq)
{
    assert((sq >= A8 && sq <= H1) || sq == NO_SQUARE);
    if (sq == NO_SQUARE || get_rank(sq) == RANK_8 || get_file(sq) == FILE_A) return NO_SQUARE;
    return (square_t)(sq - 9);
}

/**
 * \brief Is a square a light colored square?
 *
 * \param sq    A square
 *
 * \return  true if the square is light colored, otherwise false
 */
bool is_light_sq(square_t sq)
{
    assert(sq>=A8 && sq<=H1);
    rank_t r = get_rank(sq);
    file_t f = get_file(sq);
    return (r%2) == (f%2);
}

/**
 * \brief Is a square a dark colored square?
 *
 * \param sq    A square
 *
 * \return  true if the square is dark colored, otherwise false
 */
bool is_dark_sq(square_t sq)
{
    assert(sq>=A8 && sq<=H1);
    return !is_light_sq(sq);
}

/**
 * \brief Are two squares on the same diagonal?
 *
 * \param sq1   A square
 * \param sq2   Another square
 *
 * \return true if the two squares are on the same diagonal, otherwise false.
 */
bool is_diagonal(square_t sq1, square_t sq2)
{
    return rank_distance(sq1,sq2)==file_distance(sq1,sq2);
}

static dir_t get_dir_slow(square_t from, square_t to)
{
    int32_t f_diff = get_file(from) - get_file(to);
    int32_t r_diff = get_rank(from) - get_rank(to);

    if (f_diff == 0) {
        // same file
        if (r_diff < 0) return SOUTH;
        if (r_diff > 0) return NORTH;
    } else if (f_diff < 0) {
        // from is west of to
        if (r_diff == 0) return EAST;
        if (r_diff == f_diff) return SOUTHEAST;
        if (r_diff == -f_diff) return NORTHEAST;
    } else { // f_diff > 0
        // from is east of to
        if (r_diff == 0) return WEST;
        if (r_diff == f_diff) return NORTHWEST;
        if (r_diff == -f_diff) return SOUTHWEST;
    }

    return NODIR;
}

void init_squares()
{
    for (int i=0; i<8; i++) {
        bb_ranks[i] = 0;
        bb_files[i] = 0;
    }

    for (int i=0; i<64; i++) {
        square_t sq = (square_t)i;
        bb_ranks[get_rank(sq)] |= square_to_bitmap(sq);
        bb_files[get_file(sq)] |= square_to_bitmap(sq);
    }

    for (int i=0; i<64; i++) {
        for (int j=0; j<64; j++) {
            direction_tbl[i][j] = get_dir_slow((square_t)i,(square_t)j);
        }
    }
}
