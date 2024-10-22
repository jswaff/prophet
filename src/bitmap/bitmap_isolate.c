#include <prophet/bitmap.h>

#include <assert.h>

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
uint64_t isolate_bit(uint64_t val, uint32_t index)
{
    assert (index < 64);

    uint32_t n = 0;
    for (int i=0; i<64; i++) {
        uint64_t b = (uint64_t)1 << i;
        if (b & val) {
            if (n == index) {
                return b;
            }
            ++n;
        }
    }

    return 0;
}
