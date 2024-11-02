#include "movegen_internal.h"

#include "prophet/const.h"
#include "prophet/movegen.h"
#include "prophet/position.h"

#include "position/position_internal.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * \brief Count the number of legal moves possible in a position.
 *
 * \param pos           a pointer to a chess position
 * \param caps          whether to include captures in the count
 * \param noncaps       whether to include noncaptures in the count
 *
 * \return the number of legal moves
 */
uint32_t num_legal_moves(const position_t* pos, bool caps, bool noncaps)
{
    move_t moves[MAX_MOVES_PER_PLY];
    move_t *endp = gen_legal_moves(moves, pos, caps, noncaps);

    /* count the number of moves to choose from */
    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);

    uint32_t nmoves = 0;
    if (caps) nmoves += num_caps;
    if (noncaps) nmoves += num_noncaps;

    return nmoves;
}

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
void num_moves_in_list(
    const move_t* startp, const move_t* endp, int* caps, int* noncaps)
{
    *caps = 0; *noncaps = 0;

    for (const move_t* mp=startp; mp<endp; mp++) {
        if (*mp != 0) {
            if (is_capture(*mp) || get_promopiece(*mp)) {
                (*caps)++;
            } else {
                (*noncaps)++;
            }
        }
    }
}

/**
 * \brief Given position \p pos, is \p player in check?
 *
 * \param pos           a pointer to a chess position
 * \param player        a player (white or black)
 *
 * \return true if the player is in check, otherwise false
 */
bool in_check(const position_t* pos, color_t player)
{
    square_t king_sq = player==WHITE ? pos->white_king : pos->black_king;

    return attacked(pos, king_sq, opposite_player(player));
}

/**
 * \brief Has the current player been checkmated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been checkmated, otherwise false
 */
bool is_checkmate(const position_t* pos)
{
    if (in_check(pos, pos->player)) {
        return num_legal_moves(pos, true, true) == 0;
    } 

    return false;
}

/**
 * \brief Has the current player been stalemated?
 *
 * \param pos           a pointer to a chess position
 *
 * \return true if the player has been stalemated, otherwise false
 */
bool is_stalemate(const position_t* pos)
{
    if (!in_check(pos, pos->player)) {
        return num_legal_moves(pos, true, true) == 0;
    } 

    return false;
}

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
bool is_legal_move(move_t mv, const position_t* pos)
{
    move_t moves[MAX_MOVES_PER_PLY];
    move_t* endp = gen_legal_moves(moves, pos, true, true);

    return move_list_contains(mv, moves, endp);
}

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
bool move_list_contains(move_t mv, const move_t* start, const move_t* end)
{
    for (const move_t* mp=start; mp<end; mp++) {
        if (clear_score(*mp) == clear_score(mv)) {
            return true;
        }
    }

    return false;
}
