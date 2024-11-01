#pragma once

#include "prophet/position.h"
#include "prophet/square.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

static const unsigned int CASTLE_WK     = 0x1;
static const unsigned int CASTLE_WQ     = 0x2;
static const unsigned int CASTLE_BK     = 0x4;
static const unsigned int CASTLE_BQ     = 0x8;
static const unsigned int CASTLE_WHITE  = 0x3;
static const unsigned int CASTLE_NOT_WK = 0xE;
static const unsigned int CASTLE_NOT_WQ = 0xD;
static const unsigned int CASTLE_NOT_BK = 0xB;
static const unsigned int CASTLE_NOT_BQ = 0x7;
static const unsigned int CASTLE_BLACK  = 0xC;
static const unsigned int CASTLE_ALL    = 0xF;


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



static inline bool can_castle_wk(const position_t* pos) 
{
    return pos->castling_rights & CASTLE_WK;
}


static inline bool can_castle_wq(const position_t* pos) 
{
    return pos->castling_rights & CASTLE_WQ;
}


static inline bool can_castle_bk(const position_t* pos) 
{
    return pos->castling_rights & CASTLE_BK;
}


static inline bool can_castle_bq(const position_t* pos) 
{
    return pos->castling_rights & CASTLE_BQ;
}


static inline bool is_empty_sq(const position_t* pos, square_t sq) 
{
    return pos->piece[sq] == NO_PIECE;
}


static inline color_t opposite_player(color_t player) 
{
    return player == WHITE ? BLACK : WHITE;
}


static inline bool is_draw50(const position_t* pos)
{
    return pos->fifty_counter >= 100;
}


/**
 * \brief Determine if a position is drawn by lack of mating material.
 *
 * From the xboard documentation:
 * Note that (in accordance with FIDE rules) only KK, KNK, KBK and KBKB with 
 * all bishops on the same color can be claimed as draws on the basis of 
 * insufficient mating material. The end-games KNNK, KBKN, KNKN and KBKB with 
 * unlike bishops do have mate positions, and cannot be claimed. Complex draws 
 * based on locked Pawn chains will not be recognized as draws by most 
 * interfaces, so do not claim in such positions, but just offer a draw or play
 * on.
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the position is drawn by lack of mating material, otherwise 
 * false.
 */
bool is_lack_of_mating_material(const position_t* pos);


/**
 * \brief Determine if a position is drawn by repetition.
 *
 * A position is drawn if it has occurred at least three times.
 *
 * \param pos           a pointer to a chess position
 * \param u             a pointer to the start of an array of undo_t's
 *                      It's expected that the array has at least enough 
 *                      capacity for the position's move count.
 * \param prev_reps     The number of previous repetitions required to 
 *                      declare the current position as a draw.
 *
 * \return true if the position is drawn by repetition, otherwise false.
 */
bool is_draw_rep(const position_t* pos, const undo_t* u, int prev_reps);


/**
 * \brief Flip a position vertically.
 *
 * \param pos           a pointer to a chess position
 *
 * \return a veritical mirror of the posiiton.
 */
position_t position_flip(const position_t* pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif 
