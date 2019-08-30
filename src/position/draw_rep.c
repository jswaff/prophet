#include <prophet/const.h>

#include <assert.h>

#include "position_internal.h"

/**
 * \brief Determine if a position is drawn by repetition.
 *
 * A position is drawn if it has occurred at least three times.
 *
 * \param pos           a pointer to a chess position
 * \param u             a pointer to the start of an array of undo_t's
 *                      It's expected that the array has at least enough 
 *                      capacity for the position's move count.
 *
 * \return true if the position is drawn by repetition, otherwise false.
 */
bool is_draw_rep(const position_t* pos, const undo_t* u)
{
    assert(pos);
    assert(u);
    assert(pos->move_counter <= MAX_HALF_MOVES_PER_GAME);
    assert(pos->fifty_counter <= pos->move_counter);
    assert(pos->hash_key == build_hash_key(pos));

    uint32_t reps = 0;
    uint32_t start_ind = pos->move_counter - pos->fifty_counter;
    for (uint32_t i = start_ind; i < pos->move_counter; i++)
    {
        if ((u + i)->hash_key == pos->hash_key)
        {
            ++reps;
        }
    }

    return reps >= 2;
}
