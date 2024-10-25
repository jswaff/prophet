#include "bitmap.h"

/**
* \brief Count the number of bits set in a 64 bit value.
*
* \return the number of bits set
*/
uint32_t popcnt(uint64_t val)
{
    uint32_t n = 0;

    while (val) {
        ++n;
        val &= val-1;
    }

    return n;
}
