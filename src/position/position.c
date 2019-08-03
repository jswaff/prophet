#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <prophet/position/position.h>

#include <prophet/hash.h>
#include <prophet/position/piece.h>
#include <prophet/util/bitmap.h>
#include <prophet/util/error.h>
#include <prophet/util/string_utils.h>

#include "position_internal.h"

/**
 * \brief Reset a chess position to the initial position.
 *
 * \param p     A pointer to the chess position to reset
 */
void reset_pos(position* p)
{
    assert(p != 0);

    set_pos(p, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

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
 * \param p     A pointer to the chess position to set
 * \param fen   The FEN description of the position
 *
 * \return true if successful, false otherwise
 */
bool set_pos(position* pos, const char* fen)
{
    char* my_fen = strdup(fen), *fen_part;
    /* keep the original to free the memory allocated by strdup before 
     * exiting. */
    char* fen_ptr = my_fen; 
    int32_t sq;

    for (sq=A8; sq<=H1; sq++) 
    {
        pos->piece[sq] = NO_PIECE;
    }
    sq = A8;

    for (int32_t i=NO_PIECE;i<=KING;i++) 
    {
        pos->piece_counts[WHITE][i] = 0;
        pos->piece_counts[BLACK][i] = 0;
    }

    pos->white_pawns = pos->white_knights = pos->white_bishops = 
        pos->white_rooks = pos->white_queens = pos->white_pieces = 0;
    pos->black_pawns = pos->black_knights = pos->black_bishops = 
        pos->black_rooks = pos->black_queens = pos->black_pieces = 0;

    /* Part 1 - Piece Placement */
    fen_part = strtok(my_fen," ");
    for (uint32_t c=0;c<strlen(fen_part);c++) 
    {
        char ch = *(fen_part+c);
        if (ch == '/') continue;
        if (ch >= '1' && ch <= '8') 
        {
            sq += ((int)ch) - 48;  /* '1' == ASC 48, '8' == ASC 56 */
        } 
        else 
        {
            int32_t piece=NO_PIECE;
            if (ch=='K') {
                pos->white_king = (square_t)sq;
                piece = KING;
            } else if (ch=='k') {
                pos->black_king = (square_t)sq;
                piece = -KING;
            } else if (ch=='Q') {
                piece = QUEEN;
            } else if (ch=='q') {
                piece = -QUEEN;
            } else if (ch=='R') {
                piece = ROOK;
            } else if (ch=='r') {
                piece = -ROOK;
            } else if (ch=='B') {
                piece = BISHOP;
            } else if (ch=='b') {
                piece = -BISHOP;
            } else if (ch=='N') {
                piece = KNIGHT;
            } else if (ch=='n') {
                piece = -KNIGHT;
            } else if (ch=='P') {
                piece = PAWN;
            } else if (ch=='p') {
                piece = -PAWN;
            }
            assert(piece != NO_PIECE);
            add_piece(pos, piece, (square_t)sq);
            sq++;
        }
    }
    assert(sq==64);
    
    /* Part 2 - Active Color */
    fen_part = strtok(NULL," ");
    if (*fen_part=='w' || *fen_part=='W') 
    {
        pos->player = WHITE;
    } 
    else if (*fen_part=='b' || *fen_part=='B') 
    {
        pos->player = BLACK;
    } 
    else 
    {
        error("active color - fen: %s\n", fen);
        return false;
    }

    /* Part 3 - Castling Availability */
    fen_part = strtok(NULL," ");
    if (!fen_part) 
    {
        error("castling availability - fen: %s\n", fen);
        return false;
    }
    pos->castling_rights = 0;
    if (strchr(fen_part,'K'))
        pos->castling_rights |= CASTLE_WK;
    if (strchr(fen_part,'Q'))
        pos->castling_rights |= CASTLE_WQ;
    if (strchr(fen_part,'k'))
        pos->castling_rights |= CASTLE_BK;
    if (strchr(fen_part,'q'))
        pos->castling_rights |= CASTLE_BQ;

    /* Part 4 - En Passant Target Square */
    fen_part = strtok(NULL," ");
    if (!fen_part) 
    {
        error("en passant target square - fen: %s\n", fen);
        return false;
    }
    pos->ep_sq = str_to_sq(fen_part);

    /* Part 5 - Halfmove clock (Note: not part of the EPD spec) */
    fen_part = strtok(NULL," ");
    if (fen_part) 
    {
        pos->fifty_counter=atoi(fen_part);
    } 
    else 
    {
        pos->fifty_counter = 0;
    }

    /* Part 6 - Full Move Counter (Note: not part of the EPD spec)
     * It starts at 1, and is incremented after each Black's move. */
    fen_part = strtok(NULL," ");
    if (fen_part) 
    {
        pos->move_counter=(atoi(fen_part)-1) * 2;
    } 
    else 
    {
        pos->move_counter = 0;
    }
    if (pos->player==BLACK) pos->move_counter++;

    pos->hash_key = build_hash_key(pos);
    pos->pawn_key = build_pawn_key(pos);

    free(fen_ptr);

    return true;
}

