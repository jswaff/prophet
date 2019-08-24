#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <stdint.h>

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
 * The indexed is zero based, and works from the least significant bit to the 
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

/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  

#endif /* ! defined _BITMAP_H_ */
