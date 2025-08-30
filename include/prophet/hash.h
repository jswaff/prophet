#pragma once

#include <prophet/move.h>

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   


/**
 * \brief Clear all hash tables
 */
void clear_hash_tables();


/**
 * \brief Clear the main hash table
 */
void clear_main_hash_table();


/**
 * \brief Clear the pawn hash table
 */
void clear_pawn_hash_table();


/**
 * \brief Get the number of collisions from the main hash table
 */
uint64_t get_main_hash_collisions();


/**
 * \brief Get the number of probes from the main hash table
 */
uint64_t get_main_hash_probes();


/**
 * \brief Get the number of hits from the main hash table
 */
uint64_t get_main_hash_hits();


/**
 * \brief Get the number of collisions from the pawn hash table
 */
uint64_t get_pawn_hash_collisions();


/**
 * \brief Get the number of probes from the pawn hash table
 */
uint64_t get_pawn_hash_probes();


/**
 * \brief Get the number of hits from the pawn hash table
 */
uint64_t get_pawn_hash_hits();


/**
 * \brief Resize the main hash table.  
 *
 * \param tbl           a pointer to hash table
 *
 * \return - 0 on success, or non-zero on failure
 */
int resize_main_hash_table(uint64_t max_size);


/**
 * \brief Resize the pawn hash table.  
 *
 * \param tbl           a pointer to hash table
 *
 * \return - 0 on success, or non-zero on failure
 */
int resize_pawn_hash_table(uint64_t max_size);


/**
 * \brief store a value in the main hash table
 *
 * \param key           a 64 bit key
 * \param val           the value to store
 *
 */
void store_main_hash_table(const char *fen, uint64_t val);


/**
 * \brief store a value in the pawn hash table
 *
 * \param key           a 64 bit key
 * \param val           the value to store
 *
 */
void store_pawn_hash_table(const char *fen, uint64_t val);


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
uint64_t probe_main_hash_table(const char *fen);


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
uint64_t probe_pawn_hash_table(const char *fen);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif    

