#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include <prophet/position/position.h>
#include <prophet/util/prng.h>
#include <prophet/util/bitmap.h>
#include <prophet/parameters.h>

#include "movegen_internal.h"

/* TODO: this is wasteful.  use a one dimensional array and precompute the 
 * offset for each square. */
static uint64_t rook_moves[64][4096]; /* 4096 = 2^12.  12=max bits for mask */

static uint64_t rook_masks[64]; /* reachable interior squares */
static uint64_t occupiers[64][4096];
static uint64_t blockers[64][4096];

static uint64_t magic_numbers[64];
static uint32_t magic_numbers_shift[64];

static void init_rook_masks();
static void init_occupiers_and_blockers();
static void init_magic_numbers();
static void init_moves_database();

/**
 * \brief Generate pseudo-legal rook moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_rook_moves(move* m, const position* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);
    uint64_t pmap = p->player==WHITE ? p->white_rooks : p->black_rooks;

    while (pmap) 
    {
        square_t sq = (square_t)get_msb(pmap);
        m = gen_rook_moves_from_sq(m, p, sq, caps, noncaps);
        pmap ^= square_to_bitmap(sq);
    }

    return m;
}

move* gen_rook_moves_from_sq(
    move* m, const position* p, square_t from, bool caps, bool noncaps)
{
    assert(m);
    assert(p);
    assert(from >= A8 && from <= H1);

    uint64_t rook_moves = get_rook_moves(
        p, from, get_target_squares(p, caps, noncaps));

    while (rook_moves) 
    {
        square_t sq = (square_t)get_msb(rook_moves);
        m = add_move(m, p, ROOK, from, sq);
        rook_moves ^= square_to_bitmap(sq);
    }

    return m;
}

/**
 * \brief Get rook moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the rook is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the rook can move to.
 */
uint64_t get_rook_moves(const position* p, square_t from, uint64_t targets)
{
    assert(p);
    assert(from >= A8 && from <= H1);

    /* ensure the magic numbers have already been initialized */
    assert(magic_numbers[from]);
    assert(magic_numbers_shift[from]);

    uint64_t occupied = (p->black_pieces | p->white_pieces) & 
        rook_masks[from];
    int magic_ind = (occupied * magic_numbers[from]) >> 
        magic_numbers_shift[from];

    return rook_moves[from][magic_ind] & targets;
}

static void init_rook_masks()
{
    for (int i=0; i<64; i++) 
    {
        rook_masks[i] = 0;

        rank_t r = get_rank((square_t)i);
        file_t f = get_file((square_t)i);

        /* if on the same rank or file.  Edges can be ignored. */
        for (int k=0; k<64; k++) 
        {
            rank_t my_r = get_rank((square_t)k);
            file_t my_f = get_file((square_t)k);
            if (i != k) 
            {
                if ((my_r==r && my_f != FILE_A && my_f != FILE_H)
                    || (my_f==f && my_r != RANK_8 && my_r != RANK_1))
                {
                    rook_masks[i] |= square_to_bitmap(k);
                }
            }
        }
    }
}

static void init_occupiers_and_blockers()
{
    for (uint32_t sq=0; sq<64; sq++) {
        uint64_t mask = rook_masks[sq];
        uint32_t num_mask_bits = popcnt(mask);
        uint32_t num_variations = 1 << num_mask_bits;
        assert(num_variations <= 4096);

        for (uint32_t i=0; i<num_variations; i++) 
        {
            occupiers[sq][i] = blockers[sq][i] = 0;

            /* map the index to an occupancy variation.  the idea is that each 
             * bit in the index maps to a bit in the mask. */
            uint32_t index = i;
            while (index) 
            {
                uint32_t index_bit = get_lsb(index);
                occupiers[sq][i] |= isolate_bit(mask, index_bit);
                index ^= 1 << index_bit;
            }

            /* create the blocker - that's the first occupied square in every 
             * direction. */
            square_t to = north((square_t)sq);
            while (to != NO_SQUARE) 
            {
                if (occupiers[sq][i] & square_to_bitmap(to)) 
                {
                    blockers[sq][i] |= square_to_bitmap(to);
                    break;
                }
                to = north(to);
            }
            to = south((square_t)sq);
            while (to != NO_SQUARE) 
            {
                if (occupiers[sq][i] & square_to_bitmap(to)) 
                {
                    blockers[sq][i] |= square_to_bitmap(to);
                    break;
                }
                to = south(to);
            }
            to = east((square_t)sq);
            while (to != NO_SQUARE) 
            {
                if (occupiers[sq][i] & square_to_bitmap(to)) 
                {
                    blockers[sq][i] |= square_to_bitmap(to);
                    break;
                }
                to = east(to);
            }
            to = west((square_t)sq);
            while (to != NO_SQUARE) 
            {
                if (occupiers[sq][i] & square_to_bitmap(to)) 
                {
                    blockers[sq][i] |= square_to_bitmap(to);
                    break;
                }
                to = west(to);
            }
        }
    }
}

static void init_magic_numbers()
{
    for (uint32_t sq=0; sq<64; sq++) 
    {
        uint64_t mask = rook_masks[sq];
        uint32_t num_mask_bits = popcnt(mask);
        uint32_t num_variations = 1 << num_mask_bits;

        uint64_t magic;
        uint32_t magic_shift = 64 - num_mask_bits;

        bool fail;
        bool is_used[num_variations];
        uint64_t used_by[num_variations];

        do 
        {
            magic = random64() & random64() & random64(); /* ~ 8 bits */

            for (uint32_t i=0; i<num_variations; i++) 
            {
                is_used[i] = false;
            }

            fail = false;
            for (uint32_t i=0; i<num_variations; i++) 
            {
                uint32_t index = (occupiers[sq][i] * magic) >> magic_shift;

                /* fail if this index is used by a blocker set that is 
                 * incorrect for this occupancy variation. */
                uint64_t blocker_set = blockers[sq][i];
                fail = is_used[index] && used_by[index] != blocker_set;
                if (fail) 
                {
                    /* this magic number doesn't work.  try again. */
                    break;
                }
                is_used[index] = true;
                used_by[index] = blocker_set;
            }
        } while (fail);

        /* at this point we have a magic number for this square that has the 
         * property: for all combinations of occupiers (interior squares 
         * reachable from this square), we can derive an index.  Any other 
         * combination of occupiers that maps to the same index has the same 
         * "blocker set". */
        magic_numbers[sq] = magic;
        magic_numbers_shift[sq] = magic_shift;
    }
}

static void init_moves_database()
{
    for (uint32_t sq=0; sq<64; sq++) 
    {
        uint64_t mask = rook_masks[sq];
        uint32_t num_mask_bits = popcnt(mask);
        uint32_t num_variations = 1 << num_mask_bits;

        for (uint32_t i=0; i<num_variations; i++) 
        {
            int magic_ind = (occupiers[sq][i] * magic_numbers[sq]) >> 
                magic_numbers_shift[sq];
            rook_moves[sq][magic_ind] =
                gen_moves_mask((square_t)sq, occupiers[sq][i], &north)
              | gen_moves_mask((square_t)sq, occupiers[sq][i], &south)
              | gen_moves_mask((square_t)sq, occupiers[sq][i], &east)
              | gen_moves_mask((square_t)sq, occupiers[sq][i], &west);
        }
    }
}

void init_rook_movegen()
{
    init_rook_masks();
    init_occupiers_and_blockers();
    init_magic_numbers();
    init_moves_database();
}
