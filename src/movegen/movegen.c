#include "prophet/movegen.h"

#include "prophet/position.h"

#include "movegen_internal.h"
#include "position/position_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>


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
 * \param n             the method will record the number of elements used in the stack
 * \param fen           a chess position
 * \param noncaps       if non-capturing moves should be generated
 * \param caps          if capturing moves should be generated
 *
 * \return a move pointer one greater than the last move produced
 */
void generate_moves_from_fen(move_t* m, int* n, const char* fen, bool caps, bool noncaps) {
    position_t pos;
    set_pos(&pos, fen);

    move_t* mp = gen_pseudo_legal_moves(m, &pos, caps, noncaps);
    *n = (mp - m);
}


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
move_t* gen_pseudo_legal_moves(move_t* m, const position_t* pos, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    m = gen_pawn_moves(m, pos, caps, noncaps);
    m = gen_knight_moves(m, pos, caps, noncaps);
    m = gen_bishop_moves(m, pos, caps, noncaps);
    m = gen_rook_moves(m, pos, caps, noncaps);
    m = gen_queen_moves(m, pos, caps, noncaps);
    m = gen_king_moves(m, pos, caps, noncaps);

    return m;
}

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
move_t* gen_legal_moves(move_t* m, const position_t* p, bool caps, bool noncaps)
{
    assert(caps || noncaps);

    position_t cp;
    memcpy(&cp, p, sizeof(position_t));

    move_t* endp = gen_pseudo_legal_moves(m, &cp, caps, noncaps);
    undo_t u;

    for (move_t* mp=m; mp<endp; mp++) {
        apply_move(&cp, *mp, &u);
        if (in_check(&cp, opposite_player(cp.player))) {
            /* mark as 'invalid' */
            *mp = 0;
        }
        undo_move(&cp, &u);
    }

    return endp;
}
