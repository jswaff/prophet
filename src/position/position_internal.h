#ifndef _POS_INTERNAL_H_
#define _POS_INTERNAL_H_

#include <prophet/position/position.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

/**
 * \brief Test two positions for equality
 *
 * In strict mode, each member must be equal.  In non-strict mode, that is 
 * relaxed somewhat - the fifty move counter and move counter are not required 
 * to be equal.
 *
 * \param p1        A pointer to a chess position
 * \param p2        A pointer to a second chess position
 * \param strict    strict mode
 *
 * \return      A boolean indicating if the positions are equal.
 */
bool pos_equals(const position* p1, const position* p2, bool strict);

/**
 * \brief Add a piece to a chess position
 *
 * \param p         A pointer to a chess position
 * \param piece     The piece to add
 * \param sq        The square to add the piece to
 */
void add_piece(position* p, int32_t piece, square_t sq);

/**
 * \brief Remove a piece from a chess position
 *
 * \param p         A pointer to a chess position
 * \param sq        The square to remove the piece from
 *
 * \return          The piece that was removed
 */
piece_t remove_piece(position* p, square_t sq);

/**
 * \brief Create a 64 bit hash signature of a chess position
 *
 * Note this is not a fast operation.  It would be more performant to 
 * incrementally update the hash signature as moves are made on the board.
 *
 * \param pos       A pointer to a chess position
 *
 * \return  A 64 bit hash signature
 */
uint64_t build_hash_key(const position* pos);


/**
 * \brief Create a 64 bit hash signature of the pawns in a chess position.
 *
 * Note this is not a fast operation.  It would be more performant to 
 * incrementally update the hash signature as pawn moves are made on the board.
 *
 * \param pos       A pointer to a chess position
 *
 * \return  A 64 bit hash signature
 */
uint64_t build_pawn_key(const position* pos);

/**
 * \brief Verify the internal consistency of a position.
 *
 * This would most commonly be used as a runtime check when in debug mode.
 *
 * All errors found are logged.  Execution is not stopped on the first error 
 * found.
 *
 * \param pos       The position to verify
 *
 * \return          boolean value indicating if the position is consistent.
 */
bool verify_pos(const position* pos);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _POS_INTERNAL_H_ */
