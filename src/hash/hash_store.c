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
    he->key = key;
    he->val = val;
}
