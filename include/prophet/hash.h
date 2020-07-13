#ifndef _HASH_H_
#define _HASH_H_

#include <prophet/position/move.h>

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   

enum hash_entry_type_t { MOVE_ONLY, LOWER_BOUND, UPPER_BOUND, EXACT_SCORE };
typedef enum hash_entry_type_t hash_entry_type_t;


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
void store_hash_entry(hash_table_t *tbl, uint64_t key, uint64_t val);


/**
 * Probe the hash table
 *
 * Probe the supplied hash table.  The index used is computed by taking the
 * key argument modulo the table capacity.
 *
 * \param tbl           a pointer to a hash table 
 * \param key           a 64 bit key
 *
 * \return - the stored value, or null if there is no value.
 */
uint64_t probe_hash(hash_table_t *tbl, uint64_t key);


/**
 * \brief Build a hash value.
 *
 * \param entry_type    the type of hash entry
 * \param depth         the depth of the search backing the score
 * \param score         the score to hash
 * \param mv            the move
 *
 * \return - the encoded value
 */
uint64_t build_hash_val(hash_entry_type_t entry_type, int32_t depth, 
    int32_t score, move_t mv);


hash_entry_type_t get_hash_entry_type(uint64_t val);


int32_t get_hash_entry_depth(uint64_t val);


int32_t get_hash_entry_score(uint64_t val);


move_t get_hash_entry_move(uint64_t val);



/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    

#endif /* ! defined _HASH_H_ */
