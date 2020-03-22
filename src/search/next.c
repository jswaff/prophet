#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/position/position.h>

#include "search_internal.h"

#include <assert.h>


static bool best_at_top(move_t* start, move_t* end);


/**
 * \brief Determine the next move to play.
 *
 * \param pos           the chess position 
 * \param m             pointer to a move pointer to be set to the selected
 *                      move
 * \param mo            the move ordering context
 *
 * \return true if a move has been selected, or false if there are no
 *    further moves.
 */
bool next(const position_t* pos, move_t** m, move_order_dto* mo)
{

    if (mo->next_stage == GEN_CAPS)
    {
        mo->next_stage = CAPTURES_PROMOS;

        assert(mo->start);

        /* generate the captures */
        mo->end = gen_pseudo_legal_moves(mo->start, pos, true, false);
        mo->current = mo->start;

        /* score the captures */
        for (move_t* mp=mo->start; mp<mo->end; mp++)
        {
            set_move_score(mp, mvvlva(*mp));
        }
    }

    if (mo->next_stage == CAPTURES_PROMOS)
    {
        if (best_at_top(mo->current, mo->end))
        {
            assert(is_capture(*mo->current) || 
                get_promopiece(*mo->current) != NO_PIECE);
            *m = mo->current;
            mo->current++;
            return true;
        }

        mo->next_stage = KILLER1;
    }

    if (mo->next_stage == KILLER1)
    {

        mo->next_stage = KILLER2;
    }

    if (mo->next_stage == KILLER2)
    {

        mo->next_stage = GEN_NONCAPS;
    }

    if (mo->next_stage == GEN_NONCAPS)
    {
        mo->next_stage = REMAINING;
        mo->end = gen_pseudo_legal_moves(mo->current, pos, false, true);
    }

    /* play them as they come */
    while (mo->current < mo->end)
    {
        *m = mo->current;
        mo->current++;
        if (**m != 0)
        {
            return true;
        }
    }

    return false;
}


static bool best_at_top(move_t* start, move_t* end)
{
    move_t* bestp = 0;
    int32_t bestscore = 0;

    for (move_t* mp=start; mp<end; mp++)
    {
        if (*mp != 0)
        {
            int32_t score = get_move_score(*mp);
            if (bestp==0 || score > bestscore)
            {
                bestp = mp;
                bestscore = score;
            }
        }
    }

    if (bestp)
    {
        move_t tmp_mv = *start;
        *start = *bestp;
        *bestp = tmp_mv;
        return true;
    }

    return false;
}