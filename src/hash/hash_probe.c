#include <prophet/hash.h>

#include <assert.h>


/**
 * Probe the hash table
 *
 * Probe the supplied hash table.  The index used is computed by taking the
 * key argument modulo the table capacity.
 *
 * \param tbl           a pointer to a hash table 
 * \param key           a 64 bit key
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_hash(const hash_table_t *tbl, uint64_t key)
{
    assert(tbl->tbl);

    uint32_t tbl_index = key % tbl->capacity;
    hash_entry_t *he = tbl->tbl + tbl_index;
    if (he->val != 0) 
    {
        /* do full signature match */
        if (he->key == key)
        {
            /* success */
            return he->val;
        }
    }

    return 0;
}
