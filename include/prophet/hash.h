#ifndef _HASH_H_
#define _HASH_H_

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   


typedef struct 
{
    uint64_t pieces[7][2][64];
    uint64_t ptm[2];
    uint64_t casting_rights[16];
    uint64_t ep[65]; /* include NO_SQUARE */
} zobrist_keys;


typedef struct 
{
    uint64_t key;
    uint64_t val;
} hash_entry_t;


typedef struct
{
    /* a pointer to the first element in the table */
    hash_entry_t *tbl;

    /* the capacity, in number of elements */
    uint32_t capacity;

} hash_table_t;


extern zobrist_keys zkeys;


/**
 * \brief Initialize all hash tables
 *
 * Allocates memory for hash tables and sets the capacity.
 *
 */
void init_hash_tables();


/**
 * \brief Free the memory allocated to each hash table
 *
 */
void free_hash_tables();


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    

#endif /* ! defined _HASH_H_ */

