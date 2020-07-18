#include <prophet/hash.h>

#include <assert.h>


/**
 * Probe the hash table
 *
 * Probe the supplied hash table.  The index used is computed by taking the
 * key argument modulo the table capacity.
 *
 * The internal "probes" counter is incremented with every probe.  If an entry
 * is found, either the "hits" or "collisions" counter is incremented depending
 * on whether the full key comparison check passed.
 *
 * \param tbl           a pointer to a hash table 
 * \param key           a 64 bit key
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_hash(hash_table_t *tbl, uint64_t key)
{
    assert(tbl->tbl);

    tbl->probes++;
    uint32_t tbl_index = key % tbl->capacity;
    hash_entry_t *he = tbl->tbl + tbl_index;
    if (he->val != 0) 
    {
        /* do full signature match */
        if (he->key == key)
        {
            /* success */
            tbl->hits++;
            return he->val;
        }
        else
        {
            tbl->collisions++;
        }
    }

    return 0;
}
