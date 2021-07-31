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
 * \param slot          which slot to return; a 0 based index
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_hash(hash_table_t *tbl, uint64_t key, int slot)
{
    assert(tbl->tbl);
    assert(slot==0 || slot==1);

    tbl->probes++;
    uint32_t tbl_index = key % tbl->capacity;
    hash_entry_t *he = tbl->tbl + tbl_index;

    if (slot==0 && he->val != 0) 
    {
        if (he->key == key)
        {
            tbl->hits++;
            return he->val;
        }
        else
        {
            tbl->collisions++;
        }
    }
    else if (slot==1 && he->val2 != 0)
    {
        if (he->key2 == key)
        {
            tbl->hits++;
            return he->val2;
        }
        else
        {
            tbl->collisions++;
        }
    }

    return 0;
}
