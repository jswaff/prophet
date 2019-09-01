#include <prophet/bitmap.h>

#include <assert.h>

static uint32_t get_lsb_slow(uint64_t val);
static uint32_t get_msb_slow(uint64_t val);

static uint32_t lsb[65536];
static uint32_t msb[65536];


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
 * \param val           the 64 bit value
 *
 * \return the bit position of the most significant bit
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
 * \brief Initialize the bitmap lsb/msb functions.
 */
void init_bitmap_lsb_msb()
{
    lsb[0] = 0; msb[0] = 0;
    for (uint32_t i=1; i<65536; i++) 
    {
        lsb[i] = get_lsb_slow(i);
        msb[i] = get_msb_slow(i);
    }
}
