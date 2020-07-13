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
    if (score > CHECKMATE-500) 
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
    else if (score < -(CHECKMATE-500)) 
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
    val |= ((uint64_t)depth) << 2;
    if (score >= 0) 
    {
        val |= ((uint64_t)score) << 18;
    } 
    else 
    {
        val |= ((uint64_t)-score) << 18;
        val |= ((uint64_t)1) << 34;
    }
    /* the left shift here clears the score as it is stored in the high
     * order 32 bits 
     */
    val |= ((uint64_t)mv) << 35;    

    return val;
}


hash_entry_type_t get_hash_entry_type(uint64_t val) 
{
    return (hash_entry_type_t)(val & 0x03);
}


int32_t get_hash_entry_depth(uint64_t val) 
{
    return (int32_t)((val >> 2) & 0xFFFF);
}


int32_t get_hash_entry_score(uint64_t val) 
{
    int32_t score = ((val >> 18) & 0xFFFF);
    if ((val >> 34) & 1) 
    {
        return -score;
    } 
    else 
    {
        return score;
    }
}


move_t get_hash_entry_move(uint64_t val) 
{
    move_t mv = val >> 35;
    assert(get_move_score(mv)==0);
    return mv;
}
