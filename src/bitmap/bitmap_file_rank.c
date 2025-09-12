#include "bitmap.h"

#include "prophet/square.h"

#include "position/square_internal.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>


static uint64_t bb_files[8];
static uint64_t bb_not_files[8];
static uint64_t bb_ranks[8];
static uint64_t bb_not_ranks[8];


uint64_t file_to_bitmap(file_t f)
{
    assert(f >= FILE_A && f <= FILE_H);

    return bb_files[f];
}


uint64_t not_file_to_bitmap(file_t f)
{
    assert(f >= FILE_A && f <= FILE_H);

    return bb_not_files[f];
}


uint64_t rank_to_bitmap(rank_t r)
{
    assert(r >= RANK_8 && r <= RANK_1);

    return bb_ranks[r];
}


uint64_t not_rank_to_bitmap(rank_t r)
{
    assert(r >= RANK_8 && r <= RANK_1);

    return bb_not_ranks[r];
}


uint64_t sq_to_file_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_files[get_file(sq)];
}


uint64_t sq_to_rank_bitmap(int32_t sq)
{
    assert(sq >= 0 && sq < 64);
    return bb_ranks[get_rank(sq)];
}


void init_bitmap_file_rank()
{
    /* initialize the arrays */
    memset(bb_files, 0, sizeof(uint64_t) * 8);
    memset(bb_ranks, 0, sizeof(uint64_t) * 8);


    for (int i=0; i<64; i++) {
        bb_files[get_file(i)] |= square_to_bitmap(i);
        bb_ranks[get_rank(i)] |= square_to_bitmap(i);
    }
    for (int i=0;i<8;i++) {
        bb_not_files[i] = ~bb_files[i];
        bb_not_ranks[i] = ~bb_ranks[i];
    }
}
