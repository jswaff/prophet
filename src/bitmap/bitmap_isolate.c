#include "bitmap.h"

#include <assert.h>
#include <stdint.h>


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
