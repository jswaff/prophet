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
 *
 * \return - the encoded value
 */
uint64_t build_hash_val(hash_entry_type_t entry_type, int32_t depth, 
    int32_t score, move_t mv)
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

    assert(depth < 256);
    val |= ((uint64_t)depth & 0xFF) << 2;

    /* TODO: replace this IF with an addition */
    if (score >= 0) 
    {
        val |= ((uint64_t)score) << 10;
    } 
    else 
    {
        val |= ((uint64_t)-score) << 10;
        val |= ((uint64_t)1) << 26;
    }
 
    val |= ((uint64_t)clear_score(mv)) << 27;  

    /* TODO: add an 8 bit age counter */

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
    int32_t score = ((val >> 10) & 0xFFFF);
    if ((val >> 26) & 1) 
    {
        return -score;
    } 
    else 
    {
        return score;
    }
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
    move_t mv = val >> 27;
    assert(get_move_score(mv)==0);
    return mv;
}
