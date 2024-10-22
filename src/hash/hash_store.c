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
    assert(tbl->mask);
    uint64_t tbl_index = key & tbl->mask;
    hash_entry_t *he = tbl->tbl + tbl_index;

    int selected_slot = -1;

    /* if any of the entries are for the same position, overwrite it. */
    for (int i=0; i<NUM_HASH_SLOTS_PER_BUCKET; i++) {
        if (he->key[i]==key) {
            selected_slot = i;
            break;
        }
    }

    /* if a slot is empty, fill it */
    if (selected_slot == -1) {
        for (int i=0; i<NUM_HASH_SLOTS_PER_BUCKET; i++) {
            if (he->key[i]==0) {
                selected_slot = i;
                break;
            }
        }
    }

    /* otherwise, if any entry is from a previous search, overwrite it. */
    if (selected_slot == -1) {
        for (int i=0; i<NUM_HASH_SLOTS_PER_BUCKET; i++) {
            if (get_hash_entry_age(he->val[i]) < get_hash_entry_age(val)) {
                selected_slot = i;
                break;
            }
        }
    }

    /* otherwise, select the entry with the lowest depth */
    if (selected_slot == -1) {
        selected_slot = 0;
        int min_depth = get_hash_entry_depth(he->val[0]);
        for (int i=1; i<NUM_HASH_SLOTS_PER_BUCKET; i++) {
            int depth = get_hash_entry_depth(he->val[i]);
            if (depth < min_depth) {
                min_depth = depth;
                selected_slot = i;
            }
        }
    }

    assert(selected_slot != -1);

    he->key[selected_slot] = key;
    he->val[selected_slot] = val;
}
