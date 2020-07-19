#include <prophet/error_codes.h>
#include <prophet/hash.h>
#include <prophet/util/output.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/* the default hash table size is 256 MB */
uint32_t hash_size = 134217728 * 2; 

hash_table_t htbl;

/* forward decls */
static int init_hash_table(hash_table_t *tbl, uint32_t size);


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
 * \brief Initialize all hash tables
 *
 * Allocates memory for hash tables and sets the capacity.
 *
 * \return - 0 on success, or non-zero on failure
 */
int init_hash_tables()
{
    return init_hash_table(&htbl, hash_size);
}


/**
 * \brief Free the memory allocated to each hash table
 *
 */
void free_hash_tables()
{
    out(stdout, "# freeing hash tables\n");
    assert(htbl.tbl);
    free(htbl.tbl);
}


/**
 * \brief Resize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param size          the number of bytes to allocate 
 *
 * \return - 0 on success, or non-zero on failure
 */
int resize_hash_table(hash_table_t *tbl, uint32_t size)
{
    assert(tbl->tbl);
   
    tbl->capacity = size / sizeof(hash_entry_t);
    hash_entry_t* new_table_ptr = (hash_entry_t*)realloc(
        tbl->tbl, tbl->capacity * sizeof(hash_entry_t));
    if (NULL == new_table_ptr)
    {
        return P4_ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    tbl->tbl = new_table_ptr;
    clear_hash_table(tbl);    

    out(stdout, "# P4 hash size: %d bytes ==> %d elements.\n", size, tbl->capacity);

    /* success */
    return 0;
}

/**
 * \brief Initialize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param size          the number of bytes to allocate 
 *
 * \return - 0 on success, or non-zero on failure
 */
static int init_hash_table(hash_table_t *tbl, uint32_t size)
{
    assert(size > 0);

    tbl->capacity = size / sizeof(hash_entry_t);
    tbl->tbl = (hash_entry_t*)malloc(tbl->capacity * sizeof(hash_entry_t));
    if (NULL == tbl->tbl)
    {
        return P4_ERROR_HASH_MEMORY_ALLOCATION_FAILURE;
    }
    clear_hash_table(tbl);    

    out(stdout, "# P4 hash size: %d bytes ==> %d elements.\n", size, tbl->capacity);

    /* success */
    return 0;
}
