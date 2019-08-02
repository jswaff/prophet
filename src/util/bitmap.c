#include <assert.h>

#include <prophet/util/bitmap.h>

static uint32_t get_lsb_slow(uint64_t val);
static uint32_t get_msb_slow(uint64_t val);

static uint32_t lsb[65536];
static uint32_t msb[65536];

/**
 * \brief Count the number of bits set in a 64 bit value.
 *
 * \return The number of bits set.
 */
uint32_t popcnt(uint64_t val)
{
    uint32_t n = 0;

    while (val) 
    {
        ++n;
        val &= val-1;
    }

    return n;
}

/**
 * \brief Get the least significant bit set in a 64 bit value.
 *
 * The index is 0 based, with the LSB having an index of 0 and the MSB an 
 * index of 63.
 *
 * It is an error to call this function with a value of 0.
 *
 * \param val   The 64 bit value
 *
 * \return the bit position of the least significant bit.
 */
uint32_t get_lsb(uint64_t val)
{
    assert(val > 0);

    if (val & 0xFFFF) 
    {
        return lsb[(uint32_t)(val & 0xFFFF)];
    }

    val >>= 16;
    if (val & 0xFFFF) 
    {
        return lsb[(uint32_t)(val & 0xFFFF)] + 16;
    }

    val >>= 16;
    if (val & 0xFFFF) 
    {
        return lsb[(uint32_t)(val & 0xFFFF)] + 32;
    }

    val >>= 16;

    return lsb[(uint32_t)val] + 48;
}

/**
 * \brief Get the most significant bit set in a 64 bit value.
 *
 * The index is 0 based, with the LSB having an index of 0 and the MSB an 
 * index of 63.
 *
 * It is an error to call this function with a value of 0.
 *
 * \param val   The 64 bit value
 *
 * \return the bit position of the most significant bit.
 */
uint32_t get_msb(uint64_t val)
{
    assert(val > 0);

    if ((val >> 48) & 0xFFFF) 
    {
        return msb[(uint32_t)((val>>48)&0xFFFF)] + 48;
    }

    if ((val >> 32) & 0xFFFF) 
    {
        return msb[(uint32_t)((val>>32)&0xFFFF)] + 32;
    }

    if ((val >> 16) & 0xFFFF) 
    {
        return msb[(uint32_t)((val>>16)&0xFFFF)] + 16;
    }

    return msb[(uint32_t)val];
}

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
 * \param val       A 64 bit value
 * \param index     The index of the bit to isolate
 *
 * \return A 64 bit value isolating a single bit.
 */
uint64_t isolate_bit(uint64_t val, uint32_t index)
{
    assert (index < 64);

    uint32_t n = 0;
    for (int i=0; i<64; i++) 
    {
        uint64_t b = (uint64_t)1 << i;
        if (b & val) 
        {
            if (n == index) 
            {
                return b;
            }
            ++n;
        }
    }

    return 0;
}

static uint32_t get_lsb_slow(uint64_t val)
{
    assert(val > 0);

    for (int i=0; i<64; i++) 
    {
        if (val & ((uint64_t)1 << i)) 
        {
            return i;
        }
    }

    return 0;
}

static uint32_t get_msb_slow(uint64_t val)
{
    assert(val > 0);

    for (int i=63; i>=0; i--) 
    {
        if (val & ((uint64_t)1 << i)) 
        {
            return i;
        }
    }

    return 0;
}

/**
 * \brief Initialize the bitmap functions.
 */
void init_bitmaps()
{
    lsb[0] = 0; msb[0] = 0;
    for (uint32_t i=1; i<65536; i++) 
    {
        lsb[i] = get_lsb_slow(i);
        msb[i] = get_msb_slow(i);
    }
}
