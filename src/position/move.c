#include <assert.h>

#include <prophet/position/move.h>

/**
 * \brief Create a non-capturing move
 *
 * \param piece     The moving piece.
 * \param from      The square the piece is moving from.
 * \param to        The square the piece is moving to.
 *
 * \return - the move
 */
move to_move(piece_t piece, square_t from, square_t to)
{
    return from | (to << 6) | (piece << 12);
}

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
move to_capture(piece_t piece, square_t from, square_t to, piece_t captured_piece)
{
    move mv = from | (to << 6) | (piece << 12);
    set_capture(&mv, captured_piece);
    return mv;
}

/**
 * \brief Set the promotion piece on an existing move.
 *
 * \param mv        A pointer to an existing move
 * \param promo     The piece the promoted pawn should be converted to.
 */
void set_promopiece(move *mv, piece_t promo)
{
    *mv &= ~(7 << 15);
    *mv |= promo << 15;
}

/**
 * \brief Set the captured piece on an existing move.
 *
 * \param mv        A pointer to an existing move
 * \param promo     The captured piece
 */
void set_capture(move* mv, piece_t captured_piece)
{
    assert(captured_piece != NO_PIECE);
    int32_t cp = (int)captured_piece < 0 ? -captured_piece : captured_piece;
    assert(cp > 0 && cp < 8);
    *mv &= ~(7 << 18);
    *mv |= cp << 18;
}

/**
 * \brief Set the en-passant capture indicator on an existing move.
 *
 * \param  mv       A pointer to an existing move.
 */
void set_epcapture(move *mv)
{
    set_capture(mv, PAWN);
    *mv |= 1 << 21;
}

/**
 * \brief Set the castle indicator on an existing move.
 *
 * \param mv        A pointer to an existing move.
 */
void set_castle(move *mv)
{
    *mv |= 1 << 22;
}

/**
 * \brief Get the "from" square from a move.
 *
 * \param mv       The move
 *
 * \return - the from square
 */
square_t get_from_sq(move mv)
{
    return (square_t)(mv & 63);
}

/**
 * \brief Get the "to" square from a move.
 *
 * \param mv       The move
 *
 * \return - the to square
 */
square_t get_to_sq(move mv)
{
    return (square_t)((mv >> 6) & 63);
}

/**
 * \brief Get the moving piece from a move.
 *
 * \param mv       The move
 *
 * \return - the moving piece
 */
piece_t get_piece(move mv)
{
    return (piece_t)((mv >> 12) & 7);
}

/**
 * \brief Get the captured piece from a move.
 *
 * \param mv       The move
 *
 * \return - the captured piece
 */
piece_t get_captured_piece(move mv)
{
    return (piece_t)((mv >> 18) & 7);
}

/**
 * \brief Get the promotion piece from a move.
 *
 * \param mv       The move
 *
 * \return - the promotion piece
 */
piece_t get_promopiece(move mv)
{
    return (piece_t)((mv >> 15) & 7);
}

/**
 * \brief Is move \p mv a capturing move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a capturing move, otherwise false.
 */
bool is_capture(move mv)
{
    piece_t cp = (piece_t)((mv >> 18) & 7);
    return cp != NO_PIECE;
}

/**
 * \brief Is move \p mv an en-passant move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a en-passant move, otherwise false.
 */
bool is_epcapture(move mv)
{
    return (mv >> 21) & 1;
}

/**
 * \brief Is move \p mv a castling move?
 *
 * \param mv       The move
 *
 * \return - true if the move is a castling move, otherwise false.
 */
bool is_castle(move mv)
{
    return (mv >> 22) & 1;
}

/**
 * \brief Get the score from a move.
 *
 * \param mv       The move
 *
 * \return - the score
 */
int32_t get_move_score(move mv)
{
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
 * \param m         A pointer to a move.
 * \param score     The score
 */
void set_move_score(move* m, int32_t score)
{
    // clear high order bits
    *m &= 0xFFFFFFFF;

    // now set score
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
 * \param mv       The move
 *
 * \return - A new move without a score.
 */
move clear_score(move mv)
{
    return mv & 0xFFFFFFFF;
}
