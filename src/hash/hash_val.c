#include "hash_internal.h"

#include <assert.h>
#include <stdint.h>


uint64_t build_hash_val(hash_entry_type_t entry_type, int32_t depth, int32_t score, move_t mv, uint32_t hash_age)
{
    /* convert mate scores */
    if (score >= CHECKMATE-500) {
        if (entry_type == UPPER_BOUND) {
            /* failing low on mate.  don't allow a cutoff, just store any associated move */
            entry_type = MOVE_ONLY;
        } else {
            /* convert to fail high */
            entry_type = LOWER_BOUND;
            score = CHECKMATE-500;
        }
    } else if (score <= -(CHECKMATE-500)) {
        if (entry_type == LOWER_BOUND) {
            /* failing high on -mate. */
            entry_type = MOVE_ONLY;
        } else {
            /* convert to fail low */
            entry_type = UPPER_BOUND;
            score = -(CHECKMATE-500);
        }
    }

    uint64_t val = (uint64_t)entry_type;
    assert(val < 4);

    /* fold in the depth */
    assert(depth >= 0);
    assert(depth < 256);
    val |= ((uint64_t)depth) << 2;

    /* fold in the score.  Note we add 32767 to make it a positive value that can be stored with 16 bits */
    assert(score >= -32767);
    assert(score <= 32767);
    val |= ((uint64_t)score + 32767) << 10;

    /* fold in the move */ 
    uint64_t hash_move = (uint64_t)clear_score(mv);
    assert((hash_move & 0xFFFFFF) == hash_move);
    val |= hash_move << 26;  

    /* note: leaving 50 and 51 unused since chess4j uses them and hash aging is part of the replacement strategy */

    /* fold in an age counter */
    assert(hash_age < 1024);
    val |= ((uint64_t)hash_age) << 52;

    return val;
}


uint64_t build_pawn_hash_val(int32_t mg_score, int32_t eg_score)
{
    assert(mg_score >= -32767);
    assert(mg_score <= 32767);

    assert(eg_score >= -32767);
    assert(eg_score <= 32767);

    uint64_t val = ((uint64_t)mg_score + 32767) << 32;
    val |= ((uint64_t)eg_score + 32767);

    return val;
}


hash_entry_type_t get_hash_entry_type(uint64_t val) 
{
    return (hash_entry_type_t)(val & 0x03);
}


int32_t get_hash_entry_depth(uint64_t val) 
{
    return (int32_t)((val >> 2) & 0xFF);
}


int32_t get_hash_entry_score(uint64_t val) 
{
    assert(get_hash_entry_type(val) != MOVE_ONLY);
    return ((val >> 10) & 0xFFFF) - 32767;
}


move_t get_hash_entry_move(uint64_t val) 
{
    return (val >> 26) & 0xFFFFFF;
}


uint32_t get_hash_entry_age(uint64_t val)
{
    return (val >> 52) & 0x3FF;
}


int32_t get_pawn_hash_entry_mg_score(uint64_t val)
{
    return ((val >> 32) & 0xFFFF) - 32767;
}


int32_t get_pawn_hash_entry_eg_score(uint64_t val)
{
    return (val & 0xFFFF) - 32767;
}
