#include <prophet/util/output.h>

#include <assert.h>

#include "position_internal.h"

static bool verify_pos_kings(const position_t* pos);
static bool verify_pos_ep(const position_t* pos);
static bool verify_piece_counts(const position_t* pos);
static bool verify_bitmaps(const position_t* pos);
static bool verify_castling_rights(const position_t* pos);
static bool verify_hash_keys(const position_t* pos);

/**
 * \brief Verify the internal consistency of a position.
 *
 * This would most commonly be used as a runtime check when in debug mode.
 *
 * All errors found are logged.  Execution is not stopped on the first error 
 * found.
 *
 * \param pos       a pointer to the position to verify
 *
 * \return boolean value indicating if the position is consistent
 */
bool verify_pos(const position_t* pos)
{
    assert(pos);

    bool retval = true;

    if (pos->player != WHITE && pos->player != BLACK) {
        error("invalid player: %d\n", pos->player);
        retval = false;
    }

    if (pos->fifty_counter > pos->move_counter) {
        error("fifty counter (%d) greater than move counter (%d).",
            pos->fifty_counter, pos->move_counter);
        retval = false;
    }

    if (!verify_pos_kings(pos)) {
        retval = false;
    }

    if (!verify_pos_ep(pos)) {
        retval = false;
    }

    if (!verify_piece_counts(pos)) {
        retval = false;
    }

    if (!verify_bitmaps(pos)) {
        retval = false;
    }

    if (!verify_castling_rights(pos)) {
        retval = false;
    }

    if (!verify_hash_keys(pos)) {
        retval = false;
    }

    return retval;
}

static bool verify_pos_kings(const position_t* pos)
{
    bool retval = true;

    square_t wkingsq = NO_SQUARE;
    for (int32_t sq=0; sq<64; sq++) {
        if (pos->piece[sq] == KING) {
            if (wkingsq != NO_SQUARE) {
                error("more than one white king found\n");
                retval = false;
            }
            wkingsq = (square_t)sq;
        }
    }

    if (wkingsq == NO_SQUARE) {
        error("did not find white king.\n");
        retval = false;
    }

    if (wkingsq != pos->white_king) {
        error("wkingsq incorrect.  should be: %d, but was: %d\n", 
            wkingsq, pos->white_king);
        retval = false;
    }

    square_t bkingsq = NO_SQUARE;
    for (int32_t sq=0; sq<64; sq++) {
        if (pos->piece[sq] == -KING) {
            if (bkingsq != NO_SQUARE) {
                error("more than one black king found\n");
                retval = false;
            }
            bkingsq = (square_t)sq;
        }
    }

    if (bkingsq == NO_SQUARE) {
        error("did not find black king.\n");
        retval = false;
    }

    if (bkingsq != pos->black_king) {
        error("bkingsq incorrect.  should be: %d, but was: %d\n", bkingsq, 
            pos->black_king);
        retval = false;
    }

    return retval;
}

static bool verify_pos_ep(const position_t* pos)
{
    bool retval = true;

    if (pos->ep_sq != NO_SQUARE) {
        if (pos->player == BLACK) { 
            /* meaning white made the EP move */
            if (get_rank(pos->ep_sq) != RANK_3) {
                error("ep_sq is %d, player is BLACK  - should be on rank 3\n");
                retval = false;
            }
            if (pos->piece[pos->ep_sq-8] != PAWN) {
                error("ep_sq is %d, player is BLACK - should be white pawn "
                      "above ep\n");
                retval = false;
            }
        } else { 
            /* black made the EP move */
            if (get_rank(pos->ep_sq) != RANK_6) {
                error("ep_sq is %d, player is WHITE  - should be on rank 6\n");
                retval = false;
            }
            if (pos->piece[pos->ep_sq+8] != -PAWN) {
                error("ep_sq is %d, player is WHITE - should be black pawn "
                      "below ep\n");
                retval = false;
            }
        }
    }

    return retval;
}

static bool verify_piece_counts(const position_t* pos)
{
    bool retval = true;


    uint32_t num_wpawns = 0, num_wrooks = 0, num_wknights = 0, 
        num_wbishops = 0, num_wqueens = 0;
    uint32_t num_bpawns = 0, num_brooks = 0, num_bknights = 0, 
        num_bbishops = 0, num_bqueens = 0;

    for (int32_t sq=0; sq<64; sq++) {
        if (pos->piece[sq]==PAWN) {
            num_wpawns++;
        } else if (pos->piece[sq]==-PAWN) {
            num_bpawns++;
        } else if (pos->piece[sq]==ROOK) {
            num_wrooks++;
        } else if (pos->piece[sq]==-ROOK) {
            num_brooks++;
        } else if (pos->piece[sq]==KNIGHT) {
            num_wknights++;
        } else if (pos->piece[sq]==-KNIGHT) {
            num_bknights++;
        } else if (pos->piece[sq]==BISHOP) {
            num_wbishops++;
        } else if (pos->piece[sq]==-BISHOP) {
            num_bbishops++;
        } else if (pos->piece[sq]==QUEEN) {
            num_wqueens++;
        } else if (pos->piece[sq]==-QUEEN) {
            num_bqueens++;
        }
    }

    if (num_wpawns != pos->piece_counts[WHITE][PAWN]) {
        error("incorrect wpawn count: %d.  found: %d\n", 
            pos->piece_counts[WHITE][PAWN], num_wpawns);
        retval = false;
    }

    if (num_bpawns != pos->piece_counts[BLACK][PAWN]) {
        error("incorrect bpawn count: %d.  found: %d\n", 
            pos->piece_counts[BLACK][PAWN], num_bpawns);
        retval = false;
    }

    if (num_wknights != pos->piece_counts[WHITE][KNIGHT]) {
        error("incorrect wknight count: %d.  found: %d\n", 
            pos->piece_counts[WHITE][KNIGHT], num_wknights);
        retval = false;
    }

    if (num_bknights != pos->piece_counts[BLACK][KNIGHT]) {
        error("incorrect bknight count: %d.  found: %d\n", 
            pos->piece_counts[BLACK][KNIGHT], num_bknights);
        retval = false;
    }

    if (num_wbishops != pos->piece_counts[WHITE][BISHOP]) {
        error("incorrect wbishop count: %d.  found: %d\n", 
            pos->piece_counts[WHITE][BISHOP], num_wbishops);
        retval = false;
    }

    if (num_bbishops != pos->piece_counts[BLACK][BISHOP]) {
        error("incorrect bbishop count: %d.  found: %d\n", 
            pos->piece_counts[BLACK][BISHOP], num_bbishops);
        retval = false;
    }

    if (num_wrooks != pos->piece_counts[WHITE][ROOK]) {
        error("incorrect wrook count: %d.  found: %d\n", 
            pos->piece_counts[WHITE][ROOK], num_wrooks);
        retval = false;
    }

    if (num_brooks != pos->piece_counts[BLACK][ROOK]) {
        error("incorrect brook count: %d.  found: %d\n", 
            pos->piece_counts[BLACK][ROOK], num_brooks);
        retval = false;
    }

    if (num_wqueens != pos->piece_counts[WHITE][QUEEN]) {
        error("incorrect wqueen count: %d.  found: %d\n", 
            pos->piece_counts[WHITE][QUEEN], num_wqueens);
        retval = false;
    }

    if (num_bqueens != pos->piece_counts[BLACK][QUEEN]) {
        error("incorrect bqueen count: %d.  found: %d\n", 
            pos->piece_counts[BLACK][QUEEN], num_bqueens);
        retval = false;
    }

    if (pos->piece_counts[WHITE][KING] != 1U) {
        error("incorrect wking count: %d\n", 
            pos->piece_counts[WHITE][KING]);
        retval = false;
    }

    if (pos->piece_counts[BLACK][KING] != 1U) {
        error("incorrect bking count: %d\n", 
            pos->piece_counts[BLACK][KING]);
        retval = false;
    }

    return retval;
}

static bool verify_bitmaps(const position_t* pos)
{
    bool retval = true;

    uint64_t bb_wpawns = 0, bb_wrooks = 0, bb_wknights = 0, bb_wbishops = 0, 
        bb_wqueens = 0, bb_wpieces = 0;
    uint64_t bb_bpawns = 0, bb_brooks = 0, bb_bknights = 0, bb_bbishops = 0, 
        bb_bqueens = 0, bb_bpieces = 0;

    for (int32_t sq=0; sq<64; sq++) {
        uint64_t bb_sq = square_to_bitmap(sq);

        if (pos->piece[sq] > NO_PIECE) {
            bb_wpieces |= bb_sq;
        } else if (pos->piece[sq] < NO_PIECE) {
            bb_bpieces |= bb_sq;
        }

        if (pos->piece[sq] == PAWN) {
            bb_wpawns |= bb_sq;
        } else if (pos->piece[sq] == -PAWN) {
            bb_bpawns |= bb_sq;
        } else if (pos->piece[sq] == ROOK) {
            bb_wrooks |= bb_sq;
        } else if (pos->piece[sq] == -ROOK) {
            bb_brooks |= bb_sq;
        } else if (pos->piece[sq] == KNIGHT) {
            bb_wknights |= bb_sq;
        } else if (pos->piece[sq] == -KNIGHT) {
            bb_bknights |= bb_sq;
        } else if (pos->piece[sq] == BISHOP) {
            bb_wbishops |= bb_sq;
        } else if (pos->piece[sq] == -BISHOP) {
            bb_bbishops |= bb_sq;
        } else if (pos->piece[sq] == QUEEN) {
            bb_wqueens |= bb_sq;
        } else if (pos->piece[sq] == -QUEEN) {
            bb_bqueens |= bb_sq;
        }
    }

    if (bb_wpawns != pos->white_pawns) {
        error("invalid wpawns bitmap\n");
        retval = false;
    }

    if (bb_bpawns != pos->black_pawns) {
        error("invalid bpawns bitmap\n");
        retval = false;
    }

    if (bb_wknights != pos->white_knights) {
        error("invalid wknights bitmap\n");
        retval = false;
    }

    if (bb_bknights != pos->black_knights) {
        error("invalid bknights bitmap\n");
        retval = false;
    }

    if (bb_wbishops != pos->white_bishops) {
        error("invalid wbishops bitmap\n");
        retval = false;
    }

    if (bb_bbishops != pos->black_bishops) {
        error("invalid bbishops bitmap\n");
        retval = false;
    }

    if (bb_wrooks != pos->white_rooks) {
        error("invalid wrooks bitmap\n");
        retval = false;
    }

    if (bb_brooks != pos->black_rooks) {
        error("invalid brooks bitmap\n");
        retval = false;
    }

    if (bb_wqueens != pos->white_queens) {
        error("invalid wqueens bitmap\n");
        retval = false;
    }

    if (bb_bqueens != pos->black_queens) {
        error("invalid bqueens bitmap\n");
        retval = false;
    }

    if (bb_wpieces != pos->white_pieces) {
        error("invalid wpieces bitmap\n");
        retval = false;
    }

    if (bb_bpieces != pos->black_pieces) {
        error("invalid bpieces bitmap\n");
        retval = false;
    }


    return retval;
}

static bool verify_castling_rights(const position_t* pos)
{
    bool retval = true;

    if (can_castle_bq(pos)) {
        if (pos->piece[E8] != -KING) {
            error("bq castling rights but bk not on E8\n");
            retval = false;
        }
        if (pos->piece[A8] != -ROOK) {
            error("bq castling rights but br not on A8\n");
            retval = false;
        }
    }
    if (can_castle_bk(pos)) {
        if (pos->piece[E8] != -KING) {
            error("bk castling rights but bk not on E8\n");
            retval = false;
        }
        if (pos->piece[H8] != -ROOK) {
            error("bk castling rights but br not on H8\n");
            retval = false;
        }
    }
    if (can_castle_wq(pos)) {
        if (pos->piece[E1] != KING) {
            error("wq castling rights but wk not on E1\n");
            retval = false;
        }
        if (pos->piece[A1] != ROOK) {
            error("wq castling rights but wr not on A1\n");
            retval = false;
        }
    }
    if (can_castle_wk(pos)) {
        if (pos->piece[E1] != KING) {
            error("wk castling rights but wk not on E1\n");
            retval = false;
        }
        if (pos->piece[H1] != ROOK) {
            error("wk castling rights but wr not on H1\n");
            retval = false;
        }
    }

    return retval;
}

static bool verify_hash_keys(const position_t* pos)
{
    bool retval = true;

    if (pos->hash_key != build_hash_key(pos)) {
        error("invalid hash key\n");
        retval = false;
    }

    if (pos->pawn_key != build_pawn_key(pos)) {
        error("invalid pawn hash key\n");
        retval = false;
    }

    return retval;
}
