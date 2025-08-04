#pragma once

#include <stdbool.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef enum  
{
    A8,B8,C8,D8,E8,F8,G8,H8,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A1,B1,C1,D1,E1,F1,G1,H1,
    NO_SQUARE
} square_t;

typedef enum { RANK_8,RANK_7,RANK_6,RANK_5,RANK_4,RANK_3,RANK_2,RANK_1 } rank_t;

typedef enum { FILE_A,FILE_B,FILE_C,FILE_D,FILE_E,FILE_F,FILE_G,FILE_H } file_t;

typedef enum { NORTH,NORTHEAST,EAST,SOUTHEAST,SOUTH,SOUTHWEST,WEST,NORTHWEST,NODIR } dir_t;


/**
 * \brief Get the rank a square occupies.
 *
 * \param sq            a square
 *
 * \return the rank the square occupies
 */
rank_t get_rank(square_t sq);


/**
 * \brief Get the file a square occupies.
 *
 * \param sq            a square
 *
 * \return the file the square occupies
 */
file_t get_file(square_t sq);


/**
 * \brief Get the square that occupies a given rank and file.
 *
 * \param rank          a rank
 * \param file          a file
 *
 * \return the square
 */
square_t get_square(rank_t rank, file_t file);

/**
 * \brief Is a square a light colored square?
 *
 * \param sq            a square
 *
 * \return true if the square is light colored, otherwise false
 */
bool is_light_sq(square_t sq);


/**
 * \brief Is a square a dark colored square?
 *
 * \param sq            a square
 *
 * \return true if the square is dark colored, otherwise false
 */
bool is_dark_sq(square_t sq);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
