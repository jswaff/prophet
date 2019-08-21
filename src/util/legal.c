#include <stdbool.h>

#include <prophet/position/move.h>
#include <prophet/position/position.h>
#include <prophet/const.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>

/* forward decls */
static bool is_in_move_list(move mv, const move* start, const move* end);

/**
 * \brief Test move legality.
 *
 * Test that a move is legal in a given position.
 *
 * \param mv            The move to test.
 * \param pos           A pointer to the chess position.
 *
 * \returns true if legal, otherwise false.
 */
bool is_legal_move(move mv, const position* pos)
{
    move moves[MAX_MOVES_PER_PLY];
    move *endp = gen_legal_moves(moves, pos, true, true);

    return is_in_move_list(mv, moves, endp);
}

/**
 * Test if a move is a member of a list.
 *
 * The score portion of the move is ignored.
 */
static bool is_in_move_list(move mv, const move* start, const move* end)
{
    for (const move* mp=start; mp<end; mp++) 
    {
        if (clear_score(*mp) == clear_score(mv)) 
        {
            return true;
        }
    }

    return false;
}
