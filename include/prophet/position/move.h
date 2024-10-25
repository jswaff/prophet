#pragma once

#include <prophet/const.h>
#include <prophet/position/piece.h>
#include <prophet/position/square.h>

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef uint64_t move_t;

typedef struct
{
    int n; /* the number of moves in the line */
    move_t mv[MAX_PLY];    
} move_line_t;


/**
 * \brief Create a non-capturing move
 *
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 *
 * \return the move
 */
move_t to_move(piece_t piece, square_t from, square_t to);


/**
 * \brief Create a capturing move
 *
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 * \param captured_piece   the captured piece
 *
 * \return the move
 */
move_t to_capture(
    piece_t piece, square_t from, square_t to, piece_t captured_piece);


/**
 * \brief Set the promotion piece on an existing move.
 *
 * \param mv            a pointer to an existing move
 * \param promo         the piece the promoted pawn should be converted to
 */
void set_promopiece(move_t* mv, piece_t promo);


/**
 * \brief Set the captured piece on an existing move.
 *
 * \param mv            a pointer to an existing move
 * \param promo         the captured piece
 */
void set_capture(move_t* mv, piece_t captured_piece);


/**
 * \brief Set the en-passant capture indicator on an existing move.
 *
 * \param  mv           a pointer to an existing move
 */
void set_epcapture(move_t* mv);


/**
 * \brief Set the castle indicator on an existing move.
 *
 * \param  mv           a pointer to an existing move
 */
void set_castle(move_t* mv);


/**
 * \brief Get the "from" square from a move.
 *
 * \param mv            the move
 *
 * \return the from square
 */
square_t get_from_sq(move_t mv);


/**
 * \brief Get the "to" square from a move.
 *
 * \param mv            the move
 *
 * \return the to square
 */
square_t get_to_sq(move_t mv);


/**
 * \brief Get the moving piece from a move.
 *
 * \param mv            the move
 *
 * \return the moving piece
 */
piece_t get_piece(move_t mv);


/**
 * \brief Get the captured piece from a move.
 *
 * \param mv            the move
 *
 * \return the captured piece
 */
piece_t get_captured_piece(move_t mv);


/**
 * \brief Get the promotion piece from a move.
 *
 * \param mv            the move
 *
 * \return the promotion piece
 */
piece_t get_promopiece(move_t mv);


/**
 * \brief Is move \p mv a capturing move?
 *
 * \param mv            the move
 *
 * \return true if the move is a capturing move, otherwise false
 */
bool is_capture(move_t mv);


/**
 * \brief Is move \p mv an en-passant move?
 *
 * \param mv            the move
 *
 * \return true if the move is a en-passant move, otherwise false
 */
bool is_epcapture(move_t mv);


/**
 * \brief Is move \p mv a castling move?
 *
 * \param mv            the move
 *
 * \return true if the move is a castling move, otherwise false
 */
bool is_castle(move_t mv);


/**
 * \brief Get the score from a move.
 *
 * \param mv            the move
 *
 * \return the score
 */
int32_t get_move_score(move_t mv);


/**
 * \brief Set the score for a move
 *
 * \param m             a pointer to a move
 * \param score         the score
 */
void set_move_score(move_t* m, int32_t score);


/**
 * \brief Given a move \p mv, create an equivalent move without a score.
 *
 * \param mv            the move
 *
 * \return a new move without a score
 */
move_t clear_score(move_t mv);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
