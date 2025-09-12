#pragma once

#include "prophet/square.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


static const square_t flip_rank[64] =
{   A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8  
};


/**
 * @brief Create bitmap representing a single square.
 * 
 * @param sq            a square
 *
 * @return the bitmap
 */
static inline uint64_t square_to_bitmap(int sq)
{
    return (uint64_t)1 << sq;
}


/**
 * @brief Get the rank a square occupies.
 *
 * @param sq            a square
 *
 * @return the rank the square occupies
 */
rank_t get_rank(square_t sq);


/**
 * @brief Get the file a square occupies.
 *
 * @param sq            a square
 *
 * @return the file the square occupies
 */
file_t get_file(square_t sq);


/**
 * @brief Get the square that occupies a given rank and file.
 *
 * @param rank          a rank
 * @param file          a file
 *
 * @return the square
 */
square_t get_square(rank_t rank, file_t file);


/**
 * @brief Create a bitmap representation of a list of squares.
 *
 * Square A8 is the LSB, and square H1 the MSB
 *
 * @param  sq           a list of squares. Should be terminated with NO_SQUARE.
 *
 * @return a bitmap (bitboard) representation of the squares
 */
uint64_t squares_to_bitmap(square_t sq, ...);


/**
 * @brief Get the direction from one square to another.
 *
 * One of the eight compass directions, NORTH, NORTHEAST, etc will be returned, or NODIR if the two squares do not 
 * occupy the same rank, file, or diagonal. The black side of the board is considered "north".  So,
 *   get_dir(E4, E5) ==> NORTH
 *   get_dir(H8, A1) ==> SOUTHWEST
 *   get_dir(D7, E5) ==> NODIR
 *
 * @param from          the from square
 * @param to            the to square
 *
 * @return the direction of travel
 */
dir_t get_dir(square_t from, square_t to);


/**
 * @brief Get the rank distance between two squares.
 *
 * Distance is returned as an absolute value.
 *
 * @param sq1           square 1
 * @param sq2           square 2
 *
 * @return integer 0-7 representing the distance
 */
uint32_t rank_distance(square_t sq1, square_t sq2);


/**
 * @brief Get the files distance between two squares.
 *
 * Distance is returned as an absolute value.
 *
 * @param sq1           square 1
 * @param sq2           square 2
 *
 * @return integer 0-7 representing the distance
 */
uint32_t file_distance(square_t sq1, square_t sq2);


/**
 * @brief Get the distance between two squares.
 *
 * Distance is defined as the greater of rank distance and file distance.
 *
 * @param sq1           square 1
 * @param sq2           square 2
 *
 * @return integer 0-7 representing the distance
 */
uint32_t distance(square_t sq1, square_t sq2);


/**
 * @brief Get the next square to the north.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t north(square_t sq);
square_t north2(square_t sq);

/**
 * @brief Get the next square to the south.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t south(square_t sq);
square_t south2(square_t sq);


/**
 * @brief Get the next square to the east.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t east(square_t sq);


/**
 * @brief Get the next square to the west.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t west(square_t sq);


/**
 * @brief Get the next square to the northeast.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t northeast(square_t sq);


/**
 * @brief Get the next square to the southeast.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t southeast(square_t sq);


/**
 * @brief Get the next square to the southwest.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t southwest(square_t sq);


/**
 * @brief Get the next square to the northwest.
 *
 * The board is oriented with the black pieces to the north.  If the next
 * square would be off the board, NO_SQUARE is returned.
 *
 * @param sq            a square
 *
 * @return the next square
 */
square_t northwest(square_t sq);


/**
 * @brief Is a square a light colored square?
 *
 * @param sq            a square
 *
 * @return true if the square is light colored, otherwise false
 */
bool is_light_sq(square_t sq);


/**
 * @brief Is a square a dark colored square?
 *
 * @param sq            a square
 *
 * @return true if the square is dark colored, otherwise false
 */
bool is_dark_sq(square_t sq);


/**
 * @brief Are two squares on the same diagonal?
 *
 * @param sq1           a square
 * @param sq2           another square
 *
 * @return true if the two squares are on the same diagonal, otherwise false
 */
bool is_diagonal(square_t sq1, square_t sq2);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
