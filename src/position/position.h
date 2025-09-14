#pragma once

#include "prophet/move.h"
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

typedef enum { BLACK=0, WHITE=1 } color_t;

typedef int16_t nnue_accumulator_t[2][NN_SIZE_L1];

typedef struct {
    int32_t piece[64];
    uint32_t piece_counts[2][7];
    color_t player;
    square_t ep_sq;
    uint32_t castling_rights;
    uint32_t move_counter;
    uint32_t fifty_counter;
    square_t white_king;
    square_t black_king;
    uint64_t white_pawns;
    uint64_t black_pawns;
    uint64_t white_knights;
    uint64_t black_knights;
    uint64_t white_bishops;
    uint64_t black_bishops;
    uint64_t white_rooks;
    uint64_t black_rooks;
    uint64_t white_queens;
    uint64_t black_queens;
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t hash_key;
    uint64_t pawn_key;
    nnue_accumulator_t nnue_accumulator;
} position_t;


typedef struct {
    move_t mv;
    uint64_t hash_key;
    piece_t captured;
    square_t ep_sq;
    uint32_t fifty_counter;
    uint32_t castling_rights;
} undo_t;


/**
 * @brief Reset a chess position to the initial position.
 *
 * @param pos           a pointer to the chess position to reset
 */
void reset_pos(position_t *pos);


/**
 * @brief Set a chess position
 *
 * From : http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 * A FEN record contains six fields. The separator between fields is a space. The fields are:
 *
 *  1. Piece placement (from white's perspective). Each rank is described, 
 *     starting with rank 8 and ending with rank 1; within each rank, the 
 *     contents of each square are described from file a through file h. 
 *     Following the Standard Algebraic Notation (SAN), each piece is 
 *     identified by a single letter taken from the standard English names 
 *     (pawn = "P", knight = "N", bishop = "B", rook = "R", queen = "Q" and 
 *     king = "K").[1] White pieces are designated using upper-case letters 
 *     ("PNBRQK") while black pieces use lowercase ("pnbrqk"). Blank squares 
 *     are noted using digits 1 through 8 (the number of blank squares), and 
 *     "/" separate ranks.
 *
 *  2. Active color. "w" means white moves next, "b" means black.
 *
 *  3. Castling availability. If neither side can castle, this is "-". 
 *     Otherwise, this has one or more letters: "K" (White can castle 
 *     kingside), "Q" (White can castle queenside), "k" (Black can castle 
 *     kingside), and/or "q" (Black can castle queenside).
 *
 *  4. En passant target square in algebraic notation. If there's no en 
 *     passant target square, this is "-". If a pawn has just made a 2-square 
 *     move, this is the position "behind" the pawn. This is recorded 
 *     regardless of whether there is a pawn in position to make an en passant 
 *     capture.
 *
 *  5. Halfmove clock: This is the number of halfmoves since the last pawn 
 *     advance or capture. This is used to determine if a draw can be claimed 
 *     under the fifty-move rule.
 *
 *  6. Fullmove number: The number of the full move. It starts at 1, and is 
 *     incremented after Black's move.
 *
 * If an error is encountered, the position remains unchanged.
 * 
 * @param pos           a pointer to the chess position to set
 * @param fen           the FEN description of the position
 *
 * @return true if successful, false otherwise
 */
bool set_pos(position_t *pos, const char *fen);


/**
 * @brief Test two positions for equality.
 *
 * In strict mode, each member must be equal.  In non-strict mode, that is relaxed somewhat - the fifty move counter 
 * and move counter are not required to be equal.
 *
 * @param p1            a pointer to a chess position
 * @param p2            a pointer to a second chess position
 * @param strict        strict mode
 *
 * @return a boolean indicating if the positions are equal
 */
bool pos_equals(const position_t *p1, const position_t *p2, bool strict);


/**
 * @brief Add a piece to a chess position.
 *
 * @param p             a pointer to a chess position
 * @param piece         the piece to add
 * @param sq            the square to add the piece to
 */
void add_piece(position_t *p, int32_t piece, square_t sq);


/**
 * @brief Remove a piece from a chess position.
 *
 * @param p             a pointer to a chess position
 * @param sq            the square to remove the piece from
 *
 * @return the piece that was removed
 */
piece_t remove_piece(position_t *p, square_t sq);


/**
 * @brief Apply a chess move to a chess position.
 *
 * Apply a move to the position.  The move should be legal (no verification is performed). Records the information 
 * necessary to undo this move to restore the original position.
 *
 * @param pos           a pointer to a chess position
 * @param m             the move to apply
 * @param u             a pointer to an undo structure to receive the undo 
 *                      information
 */
void apply_move(position_t *pos, move_t m, undo_t *u);


/**
 * @brief Undo (reverse) a move.
 *
 * The move should be the last move played over the board.  No verification is done to ensure the position is left in 
 * a legal state.
 *
 * @param pos           a pointer to a chess position
 * @param u             a pointer to the undo information
 */
void undo_move(position_t *pos, const undo_t *u);


/**
 * @brief Create a 64 bit hash signature of a chess position
 *
 * Note this is not a fast operation.  It would be more performant to incrementally update the hash signature as moves 
 * are made on the board.
 *
 * @param pos           a pointer to a chess position
 *
 * @return a 64 bit hash signature
 */
uint64_t build_hash_key(const position_t *pos);


/**
 * @brief Create a 64 bit hash signature of the pawns in a chess position.
 *
 * Note this is not a fast operation.  It would be more performant to incrementally update the hash signature as pawn 
 * moves are made on the board.
 *
 * @param pos           a pointer to a chess position
 *
 * @return a 64 bit hash signature
 */
uint64_t build_pawn_key(const position_t *pos);


/**
 * @brief Verify the internal consistency of a position.
 *
 * This would most commonly be used as a runtime check when in debug mode.
 *
 * All errors found are logged.  Execution is not stopped on the first error found.
 *
 * @param pos           a pointer to the position to verify
 *
 * @return boolean value indicating if the position is consistent
 */
bool verify_pos(const position_t *pos);


static inline bool can_castle_w(const position_t *pos)
{
    return pos->castling_rights & CASTLE_WHITE;
}


static inline bool can_castle_wk(const position_t *pos) 
{
    return pos->castling_rights & CASTLE_WK;
}


static inline bool can_castle_wq(const position_t *pos) 
{
    return pos->castling_rights & CASTLE_WQ;
}


static inline bool can_castle_b(const position_t *pos)
{
    return pos->castling_rights & CASTLE_BLACK;
}


static inline bool can_castle_bk(const position_t *pos) 
{
    return pos->castling_rights & CASTLE_BK;
}


static inline bool can_castle_bq(const position_t *pos) 
{
    return pos->castling_rights & CASTLE_BQ;
}


static inline bool is_empty_sq(const position_t *pos, square_t sq) 
{
    return pos->piece[sq] == NO_PIECE;
}


static inline color_t opposite_player(color_t player) 
{
    return player == WHITE ? BLACK : WHITE;
}


static inline bool is_draw50(const position_t *pos)
{
    return pos->fifty_counter >= 100;
}


/**
 * @brief Determine if a position is drawn by lack of mating material.
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
 * @param pos           a pointer to a chess position
 *
 * @return true if the position is drawn by lack of mating material, otherwise 
 * false.
 */
bool is_lack_of_mating_material(const position_t *pos);


/**
 * @brief Determine if a position is drawn by repetition.
 *
 * A position is drawn if it has occurred at least three times.
 *
 * @param pos           a pointer to a chess position
 * @param u             a pointer to the start of an array of undo_t's
 *                      It's expected that the array has at least enough 
 *                      capacity for the position's move count.
 * @param prev_reps     The number of previous repetitions required to 
 *                      declare the current position as a draw.
 *
 * @return true if the position is drawn by repetition, otherwise false.
 */
bool is_draw_rep(const position_t *pos, const undo_t *u, int prev_reps);


/**
 * @brief Flip a position vertically.
 *
 * @param pos           a pointer to a chess position
 *
 * @return a veritical mirror of the posiiton.
 */
position_t position_flip(const position_t *pos);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif 
