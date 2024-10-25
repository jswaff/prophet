#pragma once

#include <prophet/position/position.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

/**
 * \brief Test two positions for equality
 *
 * In strict mode, each member must be equal.  In non-strict mode, that is 
 * relaxed somewhat - the fifty move counter and move counter are not required 
 * to be equal.
 *
 * \param p1            a pointer to a chess position
 * \param p2            a pointer to a second chess position
 * \param strict        strict mode
 *
 * \return a boolean indicating if the positions are equal
 */
bool pos_equals(const position_t* p1, const position_t* p2, bool strict);

/**
 * \brief Add a piece to a chess position
 *
 * \param p             a pointer to a chess position
 * \param piece         the piece to add
 * \param sq            the square to add the piece to
 */
void add_piece(position_t* p, int32_t piece, square_t sq);

/**
 * \brief Remove a piece from a chess position
 *
 * \param p             a pointer to a chess position
 * \param sq            the square to remove the piece from
 *
 * \return the piece that was removed
 */
piece_t remove_piece(position_t* p, square_t sq);

/**
 * \brief Create a 64 bit hash signature of a chess position
 *
 * Note this is not a fast operation.  It would be more performant to 
 * incrementally update the hash signature as moves are made on the board.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a 64 bit hash signature
 */
uint64_t build_hash_key(const position_t* pos);


/**
 * \brief Create a 64 bit hash signature of the pawns in a chess position.
 *
 * Note this is not a fast operation.  It would be more performant to 
 * incrementally update the hash signature as pawn moves are made on the board.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a 64 bit hash signature
 */
uint64_t build_pawn_key(const position_t* pos);

/**
 * \brief Verify the internal consistency of a position.
 *
 * This would most commonly be used as a runtime check when in debug mode.
 *
 * All errors found are logged.  Execution is not stopped on the first error 
 * found.
 *
 * \param pos           a pointer to the position to verify
 *
 * \return boolean value indicating if the position is consistent
 */
bool verify_pos(const position_t* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif 
