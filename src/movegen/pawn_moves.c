#include <prophet/bitmap.h>

#include <assert.h>

#include "movegen_internal.h"

static move_t* add_pawn_move(
    move_t* m, square_t from, square_t to, piece_t captured_piece, 
    bool epcapture);
static move_t* add_promotion(
    move_t* m, square_t from, square_t to, piece_t promopiece, 
    piece_t captured_piece);

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
move_t* gen_pawn_moves(move_t* m, const position_t* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    uint64_t all_pieces = p->white_pieces | p->black_pieces;
    uint64_t pmap;

    /* captures */
    if (p->player==WHITE) 
    {
        if (caps) 
        {
            uint64_t targets = p->black_pieces;
            if (p->ep_sq != NO_SQUARE) targets |= square_to_bitmap(p->ep_sq);

            /* attacks west */
            pmap = ((p->white_pawns & ~file_to_bitmap(FILE_A)) >> 9) & targets;
            while (pmap) 
            {
                square_t sq = (square_t)get_msb(pmap);
                piece_t captured = sq==p->ep_sq ? PAWN : (piece_t)p->piece[sq];
                m = add_pawn_move(
                    m, southeast(sq), sq, captured, sq==p->ep_sq);
                pmap ^= square_to_bitmap(sq);
            }

            /* attacks east */
            pmap = ((p->white_pawns & ~file_to_bitmap(FILE_H)) >> 7) & targets;
            while (pmap) 
            {
                square_t sq = (square_t)get_msb(pmap);
                piece_t captured = sq==p->ep_sq ? PAWN : (piece_t)p->piece[sq];
                m = add_pawn_move(
                    m, southwest(sq), sq, captured, sq==p->ep_sq);
                pmap ^= square_to_bitmap(sq);
            }

            /* push promotions */
            pmap = ((p->white_pawns & rank_to_bitmap(RANK_7)) >> 8) & 
                ~all_pieces;
            while (pmap) 
            {
                square_t sq = (square_t)get_msb(pmap);
                m = add_pawn_move(m, south(sq), sq, NO_PIECE, false);
                pmap ^= square_to_bitmap(sq);
            }
        }

        /* pawn pushes less promotions */
        if (noncaps) 
        {
            pmap = ((p->white_pawns & ~rank_to_bitmap(RANK_7)) >> 8) & 
                ~all_pieces;
            while (pmap) 
            {
                square_t sq = (square_t)get_msb(pmap);
                m = add_pawn_move(m, south(sq), sq, NO_PIECE, false);
                if (get_rank(sq)==RANK_3 && p->piece[north(sq)]==NO_PIECE) 
                {
                    m = add_pawn_move(
                        m, south(sq), north(sq), NO_PIECE, false);
                }
                pmap ^= square_to_bitmap(sq);
            }
        }
    } 
    else 
    {
        if (caps) 
        {
            uint64_t targets = p->white_pieces;
            if (p->ep_sq != NO_SQUARE) targets |= square_to_bitmap(p->ep_sq);

            /* attacks west */
            pmap = ((p->black_pawns & ~file_to_bitmap(FILE_A)) << 7) & targets;
            while (pmap) 
            {
                square_t sq = (square_t)get_lsb(pmap);
                piece_t captured = sq==p->ep_sq ? PAWN : (piece_t)p->piece[sq];
                m = add_pawn_move(
                    m, northeast(sq), sq, captured, sq==p->ep_sq);
                pmap ^= square_to_bitmap(sq);
            }

            /* attacks west */
            pmap = ((p->black_pawns & ~file_to_bitmap(FILE_H)) << 9) & targets;
            while (pmap) 
            {
                square_t sq = (square_t)get_lsb(pmap);
                piece_t captured = sq==p->ep_sq ? PAWN : (piece_t)p->piece[sq];
                m = add_pawn_move(
                    m, northwest(sq), sq, captured, sq==p->ep_sq);
                pmap ^= square_to_bitmap(sq);
            }

            /* push promotions */
            pmap = ((p->black_pawns & rank_to_bitmap(RANK_2)) << 8) & 
                ~all_pieces;
            while (pmap) 
            {
                square_t sq = (square_t)get_lsb(pmap);
                m = add_pawn_move(m, north(sq), sq, NO_PIECE, false);
                pmap ^= square_to_bitmap(sq);
            }
        }

        /* pawn pushes less promotions */
        if (noncaps) 
        {
            pmap = ((p->black_pawns & ~rank_to_bitmap(RANK_2)) << 8) & 
                ~all_pieces;
            while (pmap) 
            {
                square_t sq = (square_t)get_lsb(pmap);
                m = add_pawn_move(m, north(sq), sq, NO_PIECE, false);
                if (get_rank(sq)==RANK_6 && p->piece[south(sq)]==NO_PIECE) 
                {
                    m = add_pawn_move(
                        m, north(sq), south(sq), NO_PIECE, false);
                }
                pmap ^= square_to_bitmap(sq);
            }
        }
    }

    return m;
}

/**
 * \brief Add a pawn move to a move list.
 *
 * If the move is a capture then the capture flag is set.  If it is a promotion 
 * then four moves are added.
 *
 * \param m             pointer to a move stack
 * \param from          the square the pawn is moving from
 * \param to            the square the pawn is moving to
 * \param captured_piece   the captured piece.  May be NO_PIECE if not a 
 *                      capture.
 * \param epcapture     if the move is an en-passant capture
 *
 * \return move pointer one greater than the last move added.
 */
static move_t* add_pawn_move(
    move_t* m, square_t from, square_t to, piece_t captured_piece, 
    bool epcapture)
{
    rank_t r = get_rank(to);

    if (r==RANK_8 || r==RANK_1) 
    {
        m = add_promotion(m, from, to, QUEEN, captured_piece);
        m = add_promotion(m, from, to, KNIGHT, captured_piece);
        m = add_promotion(m, from, to, ROOK, captured_piece);
        m = add_promotion(m, from, to, BISHOP, captured_piece);
    } 
    else 
    {
        *m = to_move(PAWN, from, to);
        if (epcapture) 
        {
            set_epcapture(m);
        } 
        else if (captured_piece != NO_PIECE) 
        {
            set_capture(m, captured_piece);
        }
        ++m;
    }

    return m;
}

/**
 * \brief Add a pawn promotion to a move list.
 *
 * If the move is a capture then the capture flag is set.
 *
 * \param m             tointer to a move stack
 * \param from          the square the pawn is moving from
 * \param to            the square the pawn is moving to
 * \param promopiece    the piece to promote the pawn to : Q/R/B/N
 * \param captured_piece   the captured piece.  May be NO_PIECE if not a 
 *                      capture.
 *
 * \return move pointer one greater than the last move added
 */
static move_t* add_promotion(
    move_t* m, square_t from, square_t to, piece_t promopiece, 
    piece_t captured_piece)
{
    *m = to_move(PAWN, from, to);

    if (captured_piece != NO_PIECE) 
    {
        set_capture(m, captured_piece);
    }
    set_promopiece(m, promopiece);
    ++m;

    return m;
}
