#include "hash_internal.h"

#include "prophet/hash.h"
#include "prophet/position.h"

#include <assert.h>
#include <stdint.h>

extern hash_table_t htbl;
extern hash_table_t phtbl;

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
    assert(tbl->mask);

    tbl->probes++;
    uint64_t tbl_index = key & tbl->mask;
    hash_entry_t *he = tbl->tbl + tbl_index;

    for (int i=0; i<NUM_HASH_SLOTS_PER_BUCKET; i++) {
        if (he->key[i] == key) {
            tbl->hits++;
            return he->val[i];
        }
    }

    /* miss */

    return 0;
}


/**
 * \brief Probe the main hash table
 *
 * Probe the main hash table.  The index used is computed by taking the
 * key argument modulo the table capacity.
 *
 * The internal "probes" counter is incremented with every probe.  If an entry
 * is found, either the "hits" or "collisions" counter is incremented depending
 * on whether the full key comparison check passed.
 *
 * \param key           a 64 bit key
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_main_hash_table(const char *fen) {
    position_t pos;
    set_pos(&pos, fen);
    return probe_hash(&htbl, pos.hash_key);
}


/**
 * \brief Probe the pawn hash table
 *
 * Probe the pawn hash table.  The index used is computed by taking the
 * key argument modulo the table capacity.
 *
 * The internal "probes" counter is incremented with every probe.  If an entry
 * is found, either the "hits" or "collisions" counter is incremented depending
 * on whether the full key comparison check passed.
 *
 * \param key           a 64 bit key
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_pawn_hash_table(const char *fen) {
    position_t pos;
    set_pos(&pos, fen);
    return probe_hash(&phtbl, pos.pawn_key);
}
