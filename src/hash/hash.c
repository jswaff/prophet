#include <prophet/hash.h>
#include <prophet/util/prng.h>

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
 * \brief Initialize the zobrist keys
 *
 */
void init_zkeys() {

    for (int i=0;i<2;i++) 
    { 
        /* color */
        for (int j=0;j<64;j++) 
        { 
            /* squares */
            for (int k=1;k<=6;k++) 
            {
                zkeys.pieces[k][i][j] = random64();
            }
        }

        zkeys.ptm[i] = random64();
    }

    for (int i=0;i<16;i++) 
    { 
        /* castling rights */
        zkeys.casting_rights[i] = random64();
    }

    for (int i=0;i<65;i++) 
    { 
        /* ep square */
        zkeys.ep[i] = random64();
    }

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
    memset(tbl->tbl, 0, tbl->capacity * sizeof(hash_entry_t));

    printf("# hash capacity: %d\n", tbl->capacity);
}
