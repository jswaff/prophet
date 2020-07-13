#include <prophet/hash.h>

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* the default hash table size is 256 MB */
uint32_t hash_size = 134217728 * 2; 

hash_table_t htbl;

/* forward decls */
static void init_hash_table(hash_table_t *tbl, uint32_t size);


/**
 * \brief Clear a hash table
 *
 */
void clear_hash_table(hash_table_t *tbl)
{
    memset(tbl->tbl, 0, tbl->capacity * sizeof(hash_entry_t));
}


/**
 * \brief Initialize all hash tables
 *
 * Allocates memory for hash tables and sets the capacity.
 *
 */
void init_hash_tables()
{
    init_hash_table(&htbl, hash_size);
}


/**
 * \brief Free the memory allocated to each hash table
 *
 */
void free_hash_tables()
{
    printf("# freeing hash tables\n");
    assert(htbl.tbl);
    free(htbl.tbl);
}


/**
 * \brief Initialize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param size          the number of bytes to allocate 
 *
 */
static void init_hash_table(hash_table_t *tbl, uint32_t size)
{
    assert(size_mb > 0);

    tbl->capacity = size / sizeof(hash_entry_t);
    tbl->tbl = (hash_entry_t*)malloc(tbl->capacity * sizeof(hash_entry_t));
    clear_hash_table(tbl);    

    printf("# hash capacity: %d\n", tbl->capacity);
}
