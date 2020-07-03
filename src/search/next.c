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
    if (mo->next_stage == PV) 
    {
        mo->next_stage = GEN_CAPS;

        if (mo->pv_move != NO_MOVE)
        {
            assert(is_legal_move(mo->pv_move, pos));
            *m = &mo->pv_move;
            return true;
        }
    }

    if (mo->next_stage == GEN_CAPS)
    {
        mo->next_stage = CAPTURES_PROMOS;

        assert(mo->start);

        /* generate the captures */
        mo->end = gen_pseudo_legal_moves(mo->start, pos, true, false);
        mo->current = mo->start;

        /* score the captures, and remove any special moves already played */
        for (move_t* mp=mo->start; mp<mo->end; mp++)
        {
            if (*mp==mo->pv_move)
            {
                *mp = 0;
            }
            else
            {
                set_move_score(mp, mvvlva(*mp));
            }
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
        if (mo->killer1 != mo->pv_move && good_move(pos, mo->killer1))
        {
            assert(!is_capture(mo->killer1));
            *m = &mo->killer1;
            return true;
        }

    }

    if (mo->next_stage == KILLER2)
    {
        mo->next_stage = GEN_NONCAPS;
        if (mo->killer2 != mo->pv_move && mo->killer2 != mo->killer1 && 
            good_move(pos, mo->killer2))
        {
            assert(!is_capture(mo->killer2));
            *m = &mo->killer2;
            return true;
        }
    }

    if (mo->gen_noncaps)
    {
        if (mo->next_stage == GEN_NONCAPS)
        {
            mo->next_stage = REMAINING;
            mo->end = gen_pseudo_legal_moves(mo->current, pos, false, true);
            /* remove any moves already tried */
            for (move_t* mp=mo->current; mp<mo->end; mp++)
            {
                if (*mp==mo->pv_move || *mp==mo->killer1 || *mp==mo->killer2)
                {
                    *mp = 0;
                }
            }
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

