#include <prophet/hash.h>

#include <assert.h>


/**
 * \brief store a value in the hash table
 *
 * Store the value in the hash table, using an "always replace" replacement
 * strategy.  The index in the table is computed by taking the supplied
 * key modulo the table capacity.
 *
 * \param tbl           a pointer to a hash table 
 * \param key           a 64 bit key
 * \param val           the value to store
 *
 */
void store_hash_entry(const hash_table_t *tbl, uint64_t key, uint64_t val)
{
    assert(tbl->tbl);
    uint32_t tbl_index = key % tbl->capacity;
    hash_entry_t *he = tbl->tbl + tbl_index;

    /* Write to the first slot if any of these conditions are true:
     * 1. the full key is a match
     * 2. the existing entry is from an older search
     * 3. the depth of this entry is at least that of the existing entry
     *
     * Otherwise, write to the second slot. 
     */
    if (key==he->key ||
        get_hash_entry_age(val) < get_hash_entry_age(he->val) ||
        get_hash_entry_depth(val) >= get_hash_entry_depth(he->val))
    {
	    he->key = key;
	    he->val = val;
    }
    else
    {
    	he->key2 = key;
    	he->val2 = val;
    }
}
