#pragma once

#include "prophet/movegen.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef square_t (*dir_func_t)(square_t);

/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's 
 * bishops?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_bishop(const position_t* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by \p player 's king?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_king(const position_t* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's 
 * knights?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_knight(const position_t* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's 
 * pawns?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_pawn(const position_t* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's 
 * queens?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_queen(const position_t* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's 
 * rooks?
 *
 * \param p             a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked_by_rook(const position_t* p, square_t sq, color_t player);


/**
 * \brief Generate pseudo-legal bishop moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m.  It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_bishop_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_bishop_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal king moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m.  It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_king_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_king_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal knight moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_knight_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_knight_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal pawn moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_pawn_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal queen moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_queen_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_queen_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal rook moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to 
 * by \p m. It is assumed there is enough memory allocated to contain all 
 * generated moves.
 *
 * \param m             a pointer to a move stack
 * \param p             a pointer to a chess position
 * \param caps          whether capturing moves should be generated
 * \param noncaps       whether noncapturing moves should be generated
 *
 * \return move pointer one greater than the last move added
 */
move_t* gen_rook_moves(
    move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_rook_moves_from_sq(
    move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Add a non-capturing move to a move list.
 *
 * Creates a non-capturing move and adds it to a move list.
 *
 * \param m             a pointer to a move to set
 * \param p             a pointer to a chess position
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 *
 * \return the next move pointer
 */
move_t* add_move(
    move_t* m, const position_t* p, piece_t piece, square_t from, square_t to);


/**
 * \brief Get the set of target squares
 *
 * If \p caps is true, the targets include the opposite player's pieces.  If 
 * \p noncaps is set, targets includes all empty squares.  Note the two 
 * options are not mutually exclusive.
 *
 * \param p             a pointer to a chess position
 * \param caps          if capture targets should be included
 * \param noncaps       if non-capture targets should be included
 *
 * \return the set of target squares
 */
uint64_t get_target_squares(const position_t* p, bool caps, bool noncaps);


/**
 * \brief Generate a moves mask in one direction based on the set of occupied 
 * squares
 *
 * From the square \p sq, add all squares in one direction until the edge of 
 * the board or until an occupied square is encountered.
 *
 * \param sq            the square to start from
 * \param occupied      the set of occupied squares
 * \param dir_func      a function giving the next square for a given direction
 *
 * \return the set of squares that can be moved to
 */
uint64_t gen_moves_mask(square_t sq, uint64_t occupied, dir_func_t dir_func);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif   
