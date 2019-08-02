#include <assert.h>
#include <string.h>

#include "movegen_internal.h"

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
 * \param m         The start of a pre-allocated move stack.
 * \param p         The chess position
 * \param noncaps   If non-capturing moves should be generated
 * \param caps      If capturing moves should be generated
 *
 * \return - A move pointer one greater than the last move produced.
 */
move* gen_pseudo_legal_moves(
    move* m, const position* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    m = gen_pawn_moves(m, p, caps, noncaps);
    m = gen_knight_moves(m, p, caps, noncaps);
    m = gen_bishop_moves(m, p, caps, noncaps);
    m = gen_rook_moves(m, p, caps, noncaps);
    m = gen_queen_moves(m, p, caps, noncaps);
    m = gen_king_moves(m, p, caps, noncaps);

    return m;
}

/**
 * \brief Generate a list of legal moves.
 *
 * A complete list of strictly legal moves.
 *
 * It is assumed that the move stack contains enough storage for all 
 * (pseudo-legal) moves generated.  The move list will contain BADMOVE for 
 * pseudo-legal moves that were analyzed and found to be illegal.
 *
 * Note this method is significantly slower than generating pseudo-legal moves!
 *
 * \param m         The start of a pre-allocated move stack.
 * \param p         The chess position
 * \param noncaps   If non-capturing moves should be generated
 * \param caps      If capturing moves should be generated
 *
 * \return - A move pointer one greater than the last move produced.
 */
move* gen_legal_moves(move* m, const position* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    position cp;
    memcpy(&cp, p, sizeof(position));

    move* endp = gen_pseudo_legal_moves(m, &cp, caps, noncaps);
    undo u;

    for (move* mp=m; mp<endp; mp++) 
    {
        apply_move(&cp, *mp, &u);
        if (in_check(&cp, opposite_player(cp.player))) 
        {
            /* mark as 'invalid' */
            *mp = 0;
        }
        undo_move(&cp, &u);
    }

    return endp;
}

