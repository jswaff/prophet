#ifndef _MOVE_H_
#define _MOVE_H_

#include <stdbool.h>
#include <stdint.h>

#include <prophet/position/piece.h>
#include <prophet/position/square.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

typedef uint64_t move;

/**
 * \brief Create a non-capturing move
 *
 * \param piece     The moving piece.
 * \param from      The square the piece is moving from.
 * \param to        The square the piece is moving to.
 *
 * \return - the move
 */
move to_move(piece_t piece, square_t from, square_t to);

/**
 * \brief Create a capturing move
 *
 * \param piece     The moving piece.
 * \param from      The square the piece is moving from.
 * \param to        The square the piece is moving to.
 * \param captured_piece    The captured piece.
 *
 * \return - the move
 */
move to_capture(piece_t piece, square_t from, square_t to, piece_t captured_piece);

/**
 * \brief Set the promotion piece on an existing move.
 *
 * \param mv        A pointer to an existing move
 * \param promo     The piece the promoted pawn should be converted to.
 */
void set_promopiece(move* mv, piece_t promo);

/**
 * \brief Set the captured piece on an existing move.
 *
 * \param mv        A pointer to an existing move
 * \param promo     The captured piece
 */
void set_capture(move* mv, piece_t captured_piece);

/**
 * \brief Set the en-passant capture indicator on an existing move.
 *
 * \param  mv       A pointer to an existing move.
 */
void set_epcapture(move* mv);

/**
 * \brief Set the castle indicator on an existing move.
 *
 * \param  mv       A pointer to an existing move.
 */
void set_castle(move* mv);

/**
 * \brief Get the "from" square from a move.
 *
 * \param mv       The move
 *
 * \return - the from square
 */
square_t get_from_sq(move mv);

/**
 * \brief Get the "to" square from a move.
 *
 * \param mv       The move
 *
 * \return - the to square
 */
square_t get_to_sq(move mv);

/**
 * \brief Get the moving piece from a move.
 *
 * \param mv       The move
 *
 * \return - the moving piece
 */
piece_t get_piece(move mv);

/**
 * \brief Get the captured piece from a move.
 *
 * \param mv       The move
 *
 * \return - the captured piece
 */
piece_t get_captured_piece(move mv);

/**
 * \brief Get the promotion piece from a move.
 *
 * \param mv       The move
 *
 * \return - the promotion piece
 */
piece_t get_promopiece(move mv);

/**
 * \brief Is move \p mv a capturing move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a capturing move, otherwise false.
 */
bool is_capture(move mv);

/**
 * \brief Is move \p mv an en-passant move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a en-passant move, otherwise false.
 */
bool is_epcapture(move mv);

/**
 * \brief Is move \p mv a castling move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a castling move, otherwise false.
 */
bool is_castle(move mv);

/**
 * \brief Get the score from a move.
 *
 * \param mv       The move
 *
 * \return - the score
 */
int32_t get_move_score(move mv);

/**
 * \brief Set the score for a move
 *
 * \param m         A pointer to a move.
 * \param score     The score
 */
void set_move_score(move* m, int32_t score);


/**
 * \brief Given a move \p mv, create an equivalent move without a score.
 *
 * \param mv       The move
 *
 * \return - A new move without a score.
 */
move clear_score(move mv);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _MOVE_H_ */
