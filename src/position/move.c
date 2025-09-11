#include "prophet/move.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Create a non-capturing move
 *
 * \param piece         the moving piece
 * \param from          the square the piece is moving from
 * \param to            the square the piece is moving to
 *
 * \return the move
 */
move_t to_move(piece_t piece, square_t from, square_t to)
{
    return from | (to << 6) | (piece << 12);
}

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
move_t to_capture(piece_t piece, square_t from, square_t to, piece_t captured_piece)
{
    int32_t cp = (int)captured_piece < NO_PIECE ? -captured_piece : captured_piece;
    move_t mv = from | (to << 6) | (piece << 12) | (cp << 18);
    return mv;
}

/**
 * \brief Set the promotion piece on an existing move.
 *
 * \param mv            a pointer to an existing move
 * \param promo         the piece the promoted pawn should be converted to
 */
void set_promopiece(move_t *mv, piece_t promo)
{
    assert(promo==NO_PIECE || promo==QUEEN || promo==ROOK || promo==BISHOP || promo==KNIGHT);
    static uint64_t mask = ~(7 << 15);
    *mv &= mask;
    *mv |= promo << 15;
}

/**
 * \brief Set the captured piece on an existing move.
 *
 * \param mv            a pointer to an existing move
 * \param promo         the captured piece
 */
void set_capture(move_t* mv, piece_t captured_piece)
{
    assert(captured_piece > NO_PIECE);

    int32_t cp = (int)captured_piece < 0 ? -captured_piece : captured_piece;
    assert(cp > 0 && cp < 8);
    static uint64_t mask = ~(7 << 18);
    *mv &= mask;
    *mv |= cp << 18;
}

/**
 * \brief Set the en-passant capture indicator on an existing move.
 *
 * \param  mv           a pointer to an existing move
 */
void set_epcapture(move_t *mv)
{
    set_capture(mv, PAWN);
    *mv |= 1ULL << 21;
}

/**
 * \brief Set the castle indicator on an existing move.
 *
 * \param  mv           a pointer to an existing move
 */
void set_castle(move_t *mv)
{
    *mv |= 1ULL << 22;
}

/**
 * \brief Get the "from" square from a move.
 *
 * \param mv            the move
 *
 * \return the from square
 */
square_t get_from_sq(move_t mv)
{
    return (square_t)(mv & 63);
}

/**
 * \brief Get the "to" square from a move.
 *
 * \param mv            the move
 *
 * \return the to square
 */
square_t get_to_sq(move_t mv)
{
    return (square_t)((mv >> 6) & 63);
}

/**
 * \brief Get the moving piece from a move.
 *
 * \param mv            the move
 *
 * \return the moving piece
 */
piece_t get_piece(move_t mv)
{
    return (piece_t)((mv >> 12) & 7);
}

/**
 * \brief Get the captured piece from a move.
 *
 * \param mv            the move
 *
 * \return the captured piece
 */
piece_t get_captured_piece(move_t mv)
{
    return (piece_t)((mv >> 18) & 7);
}

/**
 * \brief Get the promotion piece from a move.
 *
 * \param mv            the move
 *
 * \return the promotion piece
 */
piece_t get_promopiece(move_t mv)
{
    return (piece_t)((mv >> 15) & 7);
}

/**
 * \brief Is move \p mv a capturing move?
 *
 * \param mv            the move
 *
 * \return true if the move is a capturing move, otherwise false
 */
bool is_capture(move_t mv)
{
    static uint64_t capture_mask = (1ULL << 18) | (1ULL << 19) | (1ULL << 20);
    return mv & capture_mask;
}

/**
 * \brief Is move \p mv an en-passant move?
 *
 * \param mv            the move
 *
 * \return true if the move is a en-passant move, otherwise false
 */
bool is_epcapture(move_t mv)
{
    static uint64_t ep_capture_mask = 1ULL << 21;
    return mv & ep_capture_mask;
}

/**
 * \brief Is move \p mv a castling move?
 *
 * \param mv            the move
 *
 * \return true if the move is a castling move, otherwise false
 */
bool is_castle(move_t mv)
{
    static uint64_t castle_mask = 1ULL << 22;
    return mv & castle_mask;
}

/**
 * \brief Get the score from a move.
 *
 * \param mv            the move
 *
 * \return the score
 */
int32_t get_move_score(move_t mv)
{
    //return (mv >> 32) - 32767;
    int32_t score = ((mv >> 32) & 0x7FFFFFFF);
    if ((mv >> 63) & 1) {
        return -score;
    } else {
        return score;
    }
}

/**
 * \brief Set the score for a move
 *
 * \param m             a pointer to a move
 * \param score         the score
 */
void set_move_score(move_t* m, int32_t score)
{
    assert(score >= -32767);
    assert(score <= 32767);

    /* clear high order bits */
    *m &= 0xFFFFFFFF;

    /* now set score */
    //*m |= ((uint64_t)(score+32767))<<32;
    if (score >= 0) {
        *m |= ((uint64_t)score)<<32;
    } else {
        *m |= ((uint64_t)-score)<<32;
        *m |= ((uint64_t)1)<<63;
    }
}

/**
 * \brief Given a move \p mv, create an equivalent move without a score.
 *
 * \param mv            the move
 *
 * \return A new move without a score
 */
move_t clear_score(move_t mv)
{
    return mv & 0xFFFFFFFF;
}
