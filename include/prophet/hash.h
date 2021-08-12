#ifndef _HASH_H_
#define _HASH_H_

#include <prophet/position/move.h>

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   

enum hash_entry_type_t { LOWER_BOUND, UPPER_BOUND, EXACT_SCORE, MOVE_ONLY };
typedef enum hash_entry_type_t hash_entry_type_t;


typedef struct 
{
    uint64_t pieces[7][2][64];
    uint64_t ptm[2];
    uint64_t casting_rights[16];
    uint64_t ep[65]; /* include NO_SQUARE */
} zobrist_keys;

#define NUM_HASH_SLOTS_PER_BUCKET 4
typedef struct 
{
    uint64_t key[NUM_HASH_SLOTS_PER_BUCKET];
    uint64_t val[NUM_HASH_SLOTS_PER_BUCKET];
} hash_entry_t;


typedef struct
{
    /* a pointer to the first element in the table */
    hash_entry_t *tbl;

    /* the capacity, in number of elements */
    uint32_t capacity;

    /* the number of times the table has been probed */
    uint64_t probes;

    /* the number of probes that an entry was found with a full key match */
    uint64_t hits;

    /* the number of probes that an entry was found that didn't pass the full 
     * key match 
     */
    uint64_t collisions;

} hash_table_t;


extern zobrist_keys zkeys;


/**
 * \brief Clear a hash table
 */
void clear_hash_table(hash_table_t *tbl);


/**
 * \brief Clear all hash tables
 *
 */
void clear_hash_tables();


/**
 * \brief Initialize all hash tables
 *
 * Allocates memory for hash tables and sets the capacity.
 *
 * \return - 0 on success, or non-zero on failure
*/
int init_hash_tables();


/**
 * \brief Free the memory allocated to each hash table
 */
void free_hash_tables();


/**
 * \brief Resize a hash table.  
 *
 * \param tbl           a pointer to hash table
 * \param size          the number of bytes to allocate 
 *
 * \return - 0 on success, or non-zero on failure
 */
int resize_hash_table(hash_table_t *tbl, uint32_t size);



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
void store_hash_entry(const hash_table_t *tbl, uint64_t key, uint64_t val);


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
uint64_t probe_hash(hash_table_t *tbl, uint64_t key);


/**
 * \brief Build a hash value.
 *
 * \param entry_type    the type of hash entry
 * \param depth         the depth of the search backing the score
 * \param score         the score to hash
 * \param mv            the move
 * \param hash_age      age counter
 *
 * \return - the encoded value
 */
uint64_t build_hash_val(hash_entry_type_t entry_type, int32_t depth, 
    int32_t score, move_t mv, uint32_t hash_age);


/**
 * \brief Get the hash entry type
 *
 * \param val           the hashed value
 *
 * \return - the hash entry type
 */
hash_entry_type_t get_hash_entry_type(uint64_t val);


/**
 * \brief Get the hash entry depth
 *
 * \param val           the hashed value
 *
 * \return - the hash entry depth
 */
int32_t get_hash_entry_depth(uint64_t val);


/**
 * \brief Get the hash entry score
 *
 * \param val           the hashed value
 *
 * \return - the hash entry score
 */
int32_t get_hash_entry_score(uint64_t val);


/**
 * \brief Get the hash entry move
 *
 * \param val           the hashed value
 *
 * \return - the hash entry move
 */
move_t get_hash_entry_move(uint64_t val);


/**
 * \brief Get the hash entry age
 *
 * \param val           the hashed value
 *
 * \return - the hash entry age
 */
uint32_t get_hash_entry_age(uint64_t val);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    

#endif /* ! defined _HASH_H_ */
