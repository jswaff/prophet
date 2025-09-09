#pragma once

#include <prophet/const.h>
#include <prophet/move.h>
#include <prophet/piece.h>
#include <prophet/square.h>

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   

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
 * \brief Reset a chess position to the initial position.
 *
 * \param pos           a pointer to the chess position to reset
 */
void reset_pos(position_t *pos);

/**
 * \brief Set a chess position
 *
 * From : http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
 * A FEN record contains six fields. The separator between fields is a space. 
 * The fields are:
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
 * \param pos           a pointer to the chess position to set
 * \param fen           the FEN description of the position
 *
 * \return true if successful, false otherwise
 */
bool set_pos(position_t *pos, const char *fen);


/**
 * \brief Apply a chess move to a chess position.
 *
 * Apply a move to the position.  The move should be legal (no verification is 
 * performed). Records the information necessary to undo this move to restore 
 * the original position.
 *
 * \param pos           a pointer to a chess position
 * \param m             the move to apply
 * \param u             a pointer to an undo structure to receive the undo 
 *                      information
 */
void apply_move(position_t* pos, move_t m, undo_t* u);


/**
 * \brief Undo (reverse) a move.
 *
 * The move should be the last move played over the board.  No verification 
 * is done to ensure the position is left in a legal state.
 *
 * \param pos           a pointer to a chess position
 * \param u             a pointer to the undo information
 */
void undo_move(position_t* pos, const undo_t* u);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
