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

    /* if there is an existing entry in slot 1, the depth of the new entry must
     * be greater than or equal to replace it.  If not, store in the second
     * slot. 
     */

    /* TODO: consider age */
    if (get_hash_entry_depth(val) >= get_hash_entry_depth(he->val))
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
