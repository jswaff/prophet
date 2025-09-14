#include "bitmap.h"

#include <assert.h>
#include <stdint.h>

static uint32_t get_lsb_slow(uint64_t val);

static uint32_t lsb[65536];


uint32_t get_lsb(uint64_t val)
{
    assert(val > 0);

#ifdef USE_AVX
    return __builtin_ctzll(val);
#else
    if (val & 0xFFFF) {
        return lsb[(uint32_t)(val & 0xFFFF)];
    }

    val >>= 16;
    if (val & 0xFFFF) {
        return lsb[(uint32_t)(val & 0xFFFF)] + 16;
    }

    val >>= 16;
    if (val & 0xFFFF) {
        return lsb[(uint32_t)(val & 0xFFFF)] + 32;
    }

    val >>= 16;

    return lsb[(uint32_t)val] + 48;
#endif
}


static uint32_t get_lsb_slow(uint64_t val)
{
    assert(val > 0);

    for (int i=0; i<64; i++) {
        if (val & ((uint64_t)1 << i)) {
            return i;
        }
    }

    return 0;
}


void init_bitmap_lsb()
{
    lsb[0] = 0;
    for (uint32_t i=1; i<65536; i++) {
        lsb[i] = get_lsb_slow(i);
    }
}
