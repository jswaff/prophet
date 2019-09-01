#include <prophet/bitmap.h>
#include <prophet/position/square.h>
#include <prophet/parameters.h>

#include <assert.h>
#include <string.h>

static uint64_t bb_file[8];
static uint64_t bb_rank[8];


/**
 * \brief Convert a square to a bitmap representing a file.
 *
 * \param sq            a square
 *
 * \return a bitmap
 */
uint64_t sq_to_file_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_file[get_file(sq)];
}


/**
 * \brief Convert a square to a bitmap representing a rank.
 *
 * \param sq            a square
 *
 * \return a bitmap
 */
uint64_t sq_to_rank_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_rank[get_rank(sq)];
}


/**
 * \brief Initialize the bitmap file/rank functions.
 */
void init_bitmap_file_rank()
{
    /* initialize the arrays */
    memset(bb_file, 0, sizeof(uint64_t) * 8);
    memset(bb_rank, 0, sizeof(uint64_t) * 8);


    for (int i=0; i<64; i++)
    {
        bb_file[get_file(i)] |= square_to_bitmap(i);
        bb_rank[get_rank(i)] |= square_to_bitmap(i);
    }
}
