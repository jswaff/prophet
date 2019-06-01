#ifndef _MOVEGEN_INTERNAL_H_
#define _MOVEGEN_INTERNAL_H_

#include <prophet/movegen.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

typedef square_t (*dir_func_t)(square_t);

/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's bishops?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_bishop(const position* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by \p player 's king?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_king(const position* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's knights?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_knight(const position* p, square_t sq, color_t player);

/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's pawns?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_pawn(const position* p, square_t sq, color_t player);


/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's queens?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_queen(const position* p, square_t sq, color_t player);

/**
 * \brief Given position \p p, is square \p sq attacked by one of \p player 's rooks?
 *
 * \param p         The chess position
 * \param sq        The square in question
 * \param player    The attacking player
 *
 * \return - boolean indicating if there is an attack.
 */
bool attacked_by_rook(const position* p, square_t sq, color_t player);

/**
 * \brief Generate pseudo-legal bishop moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_bishop_moves(move* m, const position* p, bool caps, bool noncaps);
move* gen_bishop_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps);

/**
 * \brief Generate pseudo-legal king moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_king_moves(move* m, const position* p, bool caps, bool noncaps);
move* gen_king_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal knight moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_knight_moves(move* m, const position* p, bool caps, bool noncaps);
move* gen_knight_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps);

/**
 * \brief Generate pseudo-legal pawn moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_pawn_moves(move* m, const position* p, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal queen moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_queen_moves(move* m, const position* p, bool caps, bool noncaps);
move* gen_queen_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps);


/**
 * \brief Generate pseudo-legal rook moves
 *
 * Moves are placed contiguously beginning at the memory location pointed to by \p m.
 * It is assumed there is enough memory allocated to contain all generated moves.
 *
 * \param m             Pointer to a move stack
 * \param p             A chess position
 * \param caps          Whether capturing moves should be generated.
 * \param noncaps       Whether noncapturing moves should be generated.
 *
 * \return  Move pointer one greater than the last move added.
 */
move* gen_rook_moves(move* m, const position* p, bool caps, bool noncaps);
move* gen_rook_moves_from_sq(move* m, const position* p, square_t from, bool caps, bool noncaps);

/**
 * \brief Get bishop moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the bishop is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the bishop can move to.
 */
uint64_t get_bishop_moves(const position* p, square_t from, uint64_t targets);


/**
 * \brief Get king moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the king is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the king can move to.
 */
uint64_t get_king_moves(square_t from, uint64_t targets);


/**
 * \brief Get knight moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the knight is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the knight can move to.
 */
uint64_t get_knight_moves(square_t from, uint64_t targets);


/**
 * \brief Get queen moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the queen is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the queen can move to.
 */
uint64_t get_queen_moves(const position* p, square_t from, uint64_t targets);


/**
 * \brief Get rook moves
 *
 * \param p             Pointer to a chess position
 * \param from          The square the rook is moving from
 * \param targets       target squares
 *
 * \return The subset of target squares the rook can move to.
 */
uint64_t get_rook_moves(const position* p, square_t from, uint64_t targets);


/**
 * \brief Add a non-capturing move to a move list
 *
 * Creates a non-capturing move and adds it to a move list.
 *
 * \param m         A pointer to a move to set
 * \param p         A pointer to a chess position
 * \param piece     The moving piece
 * \param from      The square the piece is moving from
 * \param to        The square the piece is moving to
 *
 * \return  The next move pointer
 */
move* add_move(move* m, const position* p, piece_t piece, square_t from, square_t to);

/**
 * \brief Get the set of target squares
 *
 * If \p caps is true, the targets include the opposite player's pieces.  If \p noncaps
 * is set, targets includes all empty squares.  Note the two options are not mutually
 * exclusive.
 *
 * \param p             A pointer to a chess position
 * \param caps          If capture targets should be included
 * \param noncaps       If non-capture targets should be included
 *
 * \return The set of target squares
 */
uint64_t get_target_squares(const position* p, bool caps, bool noncaps);


/**
 * \brief Generate a moves mask in one direction based on the set of occupied squares
 *
 * From the square \p sq, add all squares in one direction until the edge of the board or
 * until an occupied square is encountered.
 *
 * \param sq            The square to start from
 * \param occupied      The set of occupied squares
 * \param dir_func      A function giving the next square for a given direction
 *
 * \return  The set of squares that can be moved to.
 */
uint64_t gen_moves_mask(square_t sq, uint64_t occupied, dir_func_t dir_func);


/**
 * \brief Count the number of capture and non-capture moves
 *
 * The memory range is iterated, beginning with \p startp and ending with \p endp - 1.
 * Some slots may contain an invalid move (BADMOVE).  These "moves" are not counted.
 *
 * \param startp        The starting address of a list of moves (inclusive)
 * \param endp          The ending address of a list of moves (exclusive)
 * \param caps          A pointer to an integer to receive the number of captures
 * \param noncaps       A pointer to an integer to receive the number of noncaptures
 */
void num_moves(move* startp, move* endp, int* caps, int* noncaps);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _MOVEGEN_INTERNAL_H_ */
