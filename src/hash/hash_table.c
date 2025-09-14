#include "hash_internal.h"

#include "prophet/error_codes.h"
#include "prophet/hash.h"

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


void clear_hash_table(hash_table_t *tbl)
{
    memset(tbl->tbl, 0, tbl->capacity * sizeof(hash_entry_t));
    tbl->probes = 0;
    tbl->hits = 0;
    tbl->collisions = 0;
}


void clear_main_hash_table() {
    clear_hash_table(&htbl);
}


void clear_pawn_hash_table() {
    clear_hash_table(&phtbl);
}


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


int init_hash_tables()
{
    int retval = init_hash_table(&htbl, hash_size);
    if (0 != retval) {
        return retval;
    }
    return init_hash_table(&phtbl, pawn_hash_size);
}


void free_hash_tables()
{
    plog("# freeing hash tables\n");
    assert(htbl.tbl);
    free(htbl.tbl);
    assert(phtbl.tbl);
    free(phtbl.tbl);
}


int resize_hash_table(hash_table_t *tbl, uint64_t max_size)
{    
    assert(tbl->tbl);
    set_capacity(tbl, max_size);

    hash_entry_t* new_table_ptr = (hash_entry_t*)realloc(tbl->tbl, tbl->capacity * sizeof(hash_entry_t));
    if (NULL == new_table_ptr) {
        return ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    tbl->tbl = new_table_ptr;
    clear_hash_table(tbl);    

    /* success */
    return 0;
}


int resize_main_hash_table(uint64_t max_size) {
    return resize_hash_table(&htbl, max_size);
}


int resize_pawn_hash_table(uint64_t max_size) {
    return resize_hash_table(&phtbl, max_size);
}


static int init_hash_table(hash_table_t *tbl, uint64_t max_size)
{
    set_capacity(tbl, max_size);

    tbl->tbl = (hash_entry_t*)malloc(tbl->capacity * sizeof(hash_entry_t));
    if (NULL == tbl->tbl) {
        return ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    clear_hash_table(tbl);    


    /* success */
    return 0;
}


static void set_capacity(hash_table_t *tbl, uint64_t max_size)
{
    assert(max_size > 0);

    uint64_t max_entries = max_size / sizeof(hash_entry_t);

    /* find the largest power of 2 that is <= size */
    tbl->capacity = 1;
    while (tbl->capacity <= max_entries) {
        tbl->capacity *= 2;
    }
    tbl->capacity /= 2;

    uint64_t actual_size = tbl->capacity * sizeof(hash_entry_t);

    /* set the mask for determining the proper bucket given a key */
    tbl->mask = tbl->capacity - 1;    
}
