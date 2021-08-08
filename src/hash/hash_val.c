#include <prophet/hash.h>
#include <prophet/parameters.h>

#include <assert.h>


/**
 * \brief Build a hash value.
 *
 * \param entry_type    the type of hash entry
 * \param depth         the depth of the search backing the score
 * \param score         the score to hash
 * \param mv            the move
 * \param hash_age      age counter
 *
 * \return - the encoded value
 */
uint64_t build_hash_val(hash_entry_type_t entry_type, int32_t depth, 
    int32_t score, move_t mv, uint32_t hash_age)
{

    /* convert mate scores */
    if (score >= CHECKMATE-500) 
    {
        if (entry_type == UPPER_BOUND) 
        {
            /* failing low on mate.  don't allow a cutoff, just store any 
             * associated move
             */
            entry_type = MOVE_ONLY;
        } 
        else 
        {
            /* convert to fail high */
            entry_type = LOWER_BOUND;
            score = CHECKMATE-500;
        }
    } 
    else if (score <= -(CHECKMATE-500)) 
    {
        if (entry_type == LOWER_BOUND) 
        {
            /* failing high on -mate. */
            entry_type = MOVE_ONLY;
        } 
        else 
        {
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


    /* fold in the score.  Note we add 32767 to make it a positive
     * value that can be stored with 16 bits */
    assert(score >= -32767);
    assert(score <= 32767);
    val |= ((uint64_t)score + 32767) << 10;


    /* fold in the move */ 
    uint64_t hash_move = (uint64_t)clear_score(mv);
    assert((hash_move & 0xFFFFFF) == hash_move);
    val |= hash_move << 26;  

    /* fold in an age counter */
    assert(hash_age < 1024);
    val |= ((uint64_t)hash_age) << 50;

    return val;
}


/**
 * \brief Get the hash entry type
 *
 * \param val           the hashed value
 *
 * \return - the hash entry type
 */
hash_entry_type_t get_hash_entry_type(uint64_t val) 
{
    return (hash_entry_type_t)(val & 0x03);
}


/**
 * \brief Get the hash entry depth
 *
 * \param val           the hashed value
 *
 * \return - the hash entry depth
 */
int32_t get_hash_entry_depth(uint64_t val) 
{
    return (int32_t)((val >> 2) & 0xFF);
}


/**
 * \brief Get the hash entry score
 *
 * \param val           the hashed value
 *
 * \return - the hash entry score
 */
int32_t get_hash_entry_score(uint64_t val) 
{
    assert(get_hash_entry_type(val) != MOVE_ONLY);
    return ((val >> 10) & 0xFFFF) - 32767;
}


/**
 * \brief Get the hash entry move
 *
 * \param val           the hashed value
 *
 * \return - the hash entry move
 */
move_t get_hash_entry_move(uint64_t val) 
{
    return (val >> 26) & 0xFFFFFF;
}


/**
 * \brief Get the hash entry age
 *
 * \param val           the hashed value
 *
 * \return - the hash entry age
 */
uint32_t get_hash_entry_age(uint64_t val)
{
    return (val >> 50) & 0x3FF;
}
