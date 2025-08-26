#pragma once

#include "prophet/position.h"
#include "prophet/movegen.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef square_t (*dir_func_t)(square_t);


/**
 * \brief Generate a list of pseudo-legal moves.
 *
 * It is guaranteed that all legal moves are generated.  However, no 
 * verification is done to determine if a move would leave the current 
 * player's king in check.
 *
 * It is assumed that the move stack contains enough storage for all moves 
 * generated.
 *
 * \param m             the start of a pre-allocated move stack
 * \param pos           a pointer to a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
move_t* gen_pseudo_legal_moves(move_t *m, const position_t *pos, bool caps, bool noncaps);


/**
 * \brief Generate a list of legal moves.
 *
 * A complete list of strictly legal moves.
 *
 * It is assumed that the move stack contains enough storage for all 
 * (pseudo-legal) moves generated.  The move list will contain NO_MOVE for 
 * pseudo-legal moves that were analyzed and found to be illegal.
 *
 * Note this method is significantly slower than generating pseudo-legal moves!
 *
 * \param m             the start of a pre-allocated move stack.
 * \param pos           a pointer to a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
move_t* gen_legal_moves(move_t *m, const position_t *pos, bool caps, bool noncaps);


/**
 * \brief Add a move to a move list.
 *
 * Creates a move and adds it to a move list.  This method should not be used
 * for pawn promotions, en-passant captures, or for castling moves.
 *
 * \param m             a pointer to a move to set
 * \param p             a pointer to a chess position
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 *
 * \return the next move pointer
 */
move_t* add_move(move_t* m, const position_t* p, piece_t piece, square_t from, square_t to);


/**
 * \brief Given position \p pos, is square \p sq attacked by \p player?
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return boolean indicating if there is an attack
 */
bool attacked(const position_t *pos, square_t sq, color_t player);


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
 * \brief Given position \p pos, get a bitmap of all pieces of color \p player
 * that are attacking square \p sq.
 *
 * \param pos           a pointer to a chess position
 * \param sq            the square in question
 * \param player        the attacking player
 *
 * \return bitmap of all attacking squares
 */
uint64_t attackers(const position_t* pos, square_t sq, color_t player);


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
move_t* gen_bishop_moves(move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_bishop_moves_from_sq(move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


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
move_t* gen_king_moves(move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_king_moves_from_sq(move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


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
move_t* gen_knight_moves(move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_knight_moves_from_sq(move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


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
move_t* gen_pawn_moves(move_t* m, const position_t* p, bool caps, bool noncaps);


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
move_t* gen_queen_moves(move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_queen_moves_from_sq(move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


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
move_t* gen_rook_moves(move_t* m, const position_t* p, bool caps, bool noncaps);
move_t* gen_rook_moves_from_sq(move_t* m, const position_t* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Get bishop moves.
 *
 * \param p             a pointer to a chess position
 * \param from          The square the bishop is moving from
 *
 * \return the squares the bishop can move to
 */
uint64_t get_bishop_moves(const position_t* p, square_t from);


/**
 * \brief Get king moves.
 *
 * \param from          the square the king is moving from
 *
 * \return the squares the king can move to
 */
uint64_t get_king_moves(square_t from);


/**
 * \brief Get knight moves.
 *
 * \param from          the square the knight is moving from
 *
 * \return the squares the knight can move to
 */
uint64_t get_knight_moves(square_t from);


/**
 * \brief Get pawn attacks.
 *
 * \param from          the square the pawn is on
 * \param color         the color of the player attacking
 *
 * \return the squares the pawn attacks
 */
uint64_t get_pawn_attacks(square_t from, color_t player);


/**
 * \brief Get queen moves.
 *
 * \param p             a pointer to a chess position
 * \param from          the square the queen is moving from
 *
 * \return the squares the queen can move to
 */
uint64_t get_queen_moves(const position_t* p, square_t from);


/**
 * \brief Get rook moves.
 *
 * \param p             a pointer to a chess position
 * \param from          the square the rook is moving from
 *
 * \return the squares the rook can move to
 */
uint64_t get_rook_moves(const position_t* p, square_t from);


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


/**
 * \brief Determine if a move is "good" (pseudo-legal) in a given position.
 *
 * \param pos           a pointer to a chess position 
 * \param mv            the move to test
 * 
 * \return true if the move is good, otherwise false
 */
bool good_move(const position_t *pos, move_t mv);


/**
 * \brief Given position \p pos, is \p player in check?
 *
 * \param pos           a pointer to a chess position
 * \param player        a player (white or black)
 *
 * \return true if the player is in check, otherwise false
 */
bool in_check(const position_t *pos, color_t player);


/**
 * \brief Has the current player been checkmated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been checkmated, otherwise false
 */
bool is_checkmate(const position_t *pos);


/**
 * \brief Has the current player been stalemated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been stalemated, otherwise false
 */
bool is_stalemate(const position_t *pos);


/**
 * \brief Test if a move is a member of a list.
 *
 * Determine if a chess move is contained within a list of moves. The score 
 * portion of the move is ignored.
 *
 * \param mv            the move to look for
 * \param start         a pointer to the start of a move list
 * \param end           a pointer one past the end of a move list
 *
 * \return true if the move is contained in the list, otherwise false
 */
bool move_list_contains(move_t mv, const move_t* start, const move_t* end);


/**
 * \brief Test move legality.
 *
 * Test that a move is legal in a given position.
 *
 * \param mv            the move to test
 * \param pos           a pointer to a chess position
 *
 * \return true if legal, otherwise false
 */
bool is_legal_move(move_t mv, const position_t *pos);


/**
 * \brief Count the number of legal moves possible in a position.
 *
 * \param pos           a pointer to a chess position
 * \param caps          whether to include captures in the count
 * \param noncaps       whether to include noncaptures in the count
 *
 * \return the number of legal moves
 */
uint32_t num_legal_moves(const position_t *pos, bool caps, bool noncaps);


/**
 * \brief Count the number of capture and non-capture moves in a list.
 *
 * The memory range is iterated, beginning with \p startp and ending with 
 * \p endp - 1. Some slots may contain an invalid move (NO_MOVE).  These 
 * "moves" are not counted.
 *
 * \param startp        the starting address of a list of moves (inclusive)
 * \param endp          the ending address of a list of moves (exclusive)
 * \param caps          a pointer to an integer to receive the number of 
 *                      captures
 * \param noncaps       a pointer to an integer to receive the number of 
 *                      noncaptures
 */
void num_moves_in_list(const move_t* startp, const move_t* endp, int* caps, int* noncaps);


/**
 * \brief Count all possible moves to a fixed depth.
 *
 * The term 'perft' was first introduced by Bob Hyatt of Cray Blitz and Crafty 
 * fame. It's basically a functional test that walks the move generation tree 
 * in depth first fashion, and returning a node (vertex) count when complete.  
 * Tested from a wide range of positions, this gives a very high level of 
 * confidence that the move generation and apply functions are working 
 * correctly.
 *
 * Another use of 'perft', (and perhaps Hyatt's intention given the name) is 
 * to measure the performance of the move gen/apply systems.  By recording the 
 * time it takes to complete a perft run and the node count it returns, we have
 * a metric -- nodes per second.
 *
 * Reference: http://chessprogramming.wikispaces.com/Perft
 *
 * \param pos           a pointer to a chess position
 * \param depth         the depth of the tree from this position
 *
 * \return the number of moves generated
 */
uint64_t perft(position_t *pos, uint32_t depth);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif   
