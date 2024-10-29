#include "bitmap.h"

#include "prophet/square.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>


static uint64_t bb_files[8];
static uint64_t bb_ranks[8];

/**
 * \brief Create a bitmap representation of a file (8 squares).
 *
 * \param f             a file
 *
 * \return a bitmap representation of the file
 */
uint64_t file_to_bitmap(file_t f)
{
    assert(f >= FILE_A && f <= FILE_H);

    return bb_files[f];
}


/**
 * \brief Create a bitmap representation of a rank (8 squares).
 *
 * \param f             a rank
 *
 * \return a bitmap representation of the rank
 */
uint64_t rank_to_bitmap(rank_t r)
{
    assert(r >= RANK_8 && r <= RANK_1);

    return bb_ranks[r];
}


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
uint64_t sq_to_file_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_files[get_file(sq)];
}


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
uint64_t sq_to_rank_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_ranks[get_rank(sq)];
}


/**
 * \brief Initialize the bitmap file/rank functions.
 */
void init_bitmap_file_rank()
{
    /* initialize the arrays */
    memset(bb_files, 0, sizeof(uint64_t) * 8);
    memset(bb_ranks, 0, sizeof(uint64_t) * 8);


    for (int i=0; i<64; i++) {
        bb_files[get_file(i)] |= square_to_bitmap(i);
        bb_ranks[get_rank(i)] |= square_to_bitmap(i);
    }
}
