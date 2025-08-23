#include "prophet/hash.h"

#include "prophet/error_codes.h"

#include "util/output.h"

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* default hash table and pawn hash table sizes */
uint64_t hash_size = 1024 * 1024 * 64;         /* 64 mb */
uint64_t pawn_hash_size = 1024 * 1024 * 8;     /*  8 mb */

hash_table_t htbl;
hash_table_t phtbl;


/* forward decls */
static int init_hash_table(hash_table_t *tbl, uint64_t max_size);
static void set_capacity(hash_table_t *tbl, uint64_t max_size);

/**
 * \brief Clear a hash table
 *
 */
void clear_hash_table(hash_table_t *tbl)
{
    memset(tbl->tbl, 0, tbl->capacity * sizeof(hash_entry_t));
    tbl->probes = 0;
    tbl->hits = 0;
    tbl->collisions = 0;
}


/**
 * \brief Clear all hash tables
 *
 */
void clear_hash_tables()
{
    clear_hash_table(&htbl);
    clear_hash_table(&phtbl);
}

uint64_t get_main_hash_collisions() {
    return htbl.collisions;
}

uint64_t get_main_hash_probes() {
    return htbl.probes;
}

uint64_t get_main_hash_hits() {
    return htbl.hits;
}

uint64_t get_pawn_hash_collisions() {
    return phtbl.collisions;
}

uint64_t get_pawn_hash_probes() {
    return phtbl.probes;
}

uint64_t get_pawn_hash_hits() {
    return phtbl.hits;
}


/**
 * \brief Initialize all hash tables
 *
 * Allocates memory for hash tables and sets the capacity.
 *
 * \return - 0 on success, or non-zero on failure
 */
int init_hash_tables()
{
    int retval = init_hash_table(&htbl, hash_size);
    if (0 != retval) {
        return retval;
    }
    return init_hash_table(&phtbl, pawn_hash_size);
}


/**
 * \brief Free the memory allocated to each hash table
 *
 */
void free_hash_tables()
{
    plog("# freeing hash tables\n");
    assert(htbl.tbl);
    free(htbl.tbl);
    assert(phtbl.tbl);
    free(phtbl.tbl);
}


/**
 * \brief Resize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param size          the maximum number of bytes to allocate 
 *
 * \return - 0 on success, or non-zero on failure
 */
int resize_hash_table(hash_table_t *tbl, uint64_t max_size)
{    
    assert(tbl->tbl);
    set_capacity(tbl, max_size);

    hash_entry_t* new_table_ptr = (hash_entry_t*)realloc(tbl->tbl, tbl->capacity * sizeof(hash_entry_t));
    if (NULL == new_table_ptr) {
        return P4_ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    tbl->tbl = new_table_ptr;
    clear_hash_table(tbl);    

    /* success */
    return 0;
}

/**
 * \brief Initialize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param max_size      the maximum number of bytes to allocate 
 *
 * \return - 0 on success, or non-zero on failure
 */
static int init_hash_table(hash_table_t *tbl, uint64_t max_size)
{
    set_capacity(tbl, max_size);

    tbl->tbl = (hash_entry_t*)malloc(tbl->capacity * sizeof(hash_entry_t));
    if (NULL == tbl->tbl) {
        return P4_ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    clear_hash_table(tbl);    


    /* success */
    return 0;
}

/**
 * \brief Set the capacity and mask for a hash table.
 *
 * The capacity is set to the largest possible power of 2 to remain within the max_size.
 * Since the capacity is a power of 2, the mask will be capacity - 1, which are guaranteed
 * to be consecutive low order bits.  The mask can be applied to the hash key to quickly
 * determine the proper bucket.
 *
 * \param tbl           a pointer to hash table
 * \param max_size      the maximum number of bytes to allocate 
 *
 */

static void set_capacity(hash_table_t *tbl, uint64_t max_size)
{
    assert(max_size > 0);

    plog("# max hash size: %"PRIu64" bytes\n", max_size);
    plog("# hash entry size: %u bytes\n", sizeof(hash_entry_t));
    uint64_t max_entries = max_size / sizeof(hash_entry_t);
    plog("# max hash entries: %"PRIu64"\n", max_entries);

    /* find the largest power of 2 that is <= size */
    tbl->capacity = 1;
    while (tbl->capacity <= max_entries) {
        tbl->capacity *= 2;
    }
    tbl->capacity /= 2;

    uint64_t actual_size = tbl->capacity * sizeof(hash_entry_t);
    plog("# actual hash size: %"PRIu64" bytes\n", actual_size);
    plog("# actual hash entries: %"PRIu64"\n", tbl->capacity);

    /* set the mask for determining the proper bucket given a key */
    tbl->mask = tbl->capacity - 1;    
}
