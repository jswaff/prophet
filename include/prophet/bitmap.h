#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <prophet/position/square.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/**
* \brief Count the number of bits set in a 64 bit value.
*
* \return the number of bits set
*/
uint32_t popcnt(uint64_t val);


/**
 * \brief Get the least significant bit set in a 64 bit value.
 *
 * The index is 0 based, with the LSB having an index of 0 and the MSB an 
 * index of 63.
 *
 * It is an error to call this function with a value of 0.
 *
 * \param val           the 64 bit value
 *
 * \return the bit position of the least significant bit
 */
uint32_t get_lsb(uint64_t val);


/**
 * \brief Get the most significant bit set in a 64 bit value.
 *
 * The index is 0 based, with the LSB having an index of 0 and the MSB an 
 * index of 63.
 *
 * It is an error to call this function with a value of 0.
 *
 * \param val           the 64 bit value
 *
 * \return the bit position of the most significant bit
 */
uint32_t get_msb(uint64_t val);


/**
 * \brief Isolate a single bit in a 64 bit value.
 *
 * The index is zero based, and works from the least significant bit to the 
 * most.  For example, given a bitmap with bit pattern 11010:
 *
 *   isolate_bit(11010, 0) ==  2
 *   isolate_bit(11010, 1) ==  8
 *   isolate_bit(11010, 2) == 16
 *
 * Passing an index greater than or equal to the number of bits set will yield 
 * a return value of 0.
 *
 * \param val           a 64 bit value
 * \param index         the index of the bit to isolate
 *
 * \return a 64 bit value isolating a single bit
 */
uint64_t isolate_bit(uint64_t val, uint32_t index);


/**
 * \brief Create a bitmap representation of a file (8 squares).
 *
 * \param f             a file
 *
 * \return a bitmap representation of the file
 */
uint64_t file_to_bitmap(file_t f);


/**
 * \brief Create a bitmap representation of a rank (8 squares).
 *
 * \param f             a rank
 *
 * \return a bitmap representation of the rank
 */
uint64_t rank_to_bitmap(rank_t r);

/**
 * \brief Return a bitmap representing a ray.
 *
 * Constructs a bitmap representing the sequence of squares starting from
 * \p sq, in the direction of \p dir, to the edge of the board.
 *
 * \param sq            the starting square
 * \param dir           the direction to travel in
 *
 * \return a bitmap representation of the ray
 */
uint64_t ray(square_t sq, dir_t dir);


/**
 * \brief Convert a square to a bitmap representing a file.
 *
 * The returned bitmap can be used as a mask for isolating squares on the
 * file of the given square.
 *
 * \param sq            a square
 *
 * \return a bitmap
 */
uint64_t sq_to_file_bitmap(int32_t sq);


/**
 * \brief Convert a square to a bitmap representing a rank.
 *
 * The returned bitmap can be used as a mask for isolating squares on the
 * rank of the given square.
 *
 * \param sq            a square
 *
 * \return a bitmap
 */
uint64_t sq_to_rank_bitmap(int32_t sq);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _BITMAP_H_ */
