#include "bitmap.h"

#include <immintrin.h>
#include <stdint.h>

/**
* \brief Count the number of bits set in a 64 bit value.
*
* \return the number of bits set
*/
uint32_t popcnt(uint64_t val)
{
#ifdef USE_AVX
    return (uint32_t)_mm_popcnt_u64(val);
#else
    uint32_t n = 0;

    while (val) {
        ++n;
        val &= val-1;
    }

    return n;
#endif
}
