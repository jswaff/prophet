#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>

#include "search_internal.h"

#include <assert.h>
#include <string.h>

extern move_t killer1[MAX_PLY];
extern move_t killer2[MAX_PLY];


static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply);

static int32_t search_helper(position_t* pos, move_line_t* parent_pv, int ply, 
    int32_t depth, int32_t alpha, int32_t beta, move_t* move_stack, 
    undo_t* undo_stack, stats_t* stats);

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail);

static void add_killer(move_t killer_move, int ply);

static bool is_draw(const position_t* pos, const undo_t* undo_stack);


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param parent_pv     a pointer to the move line that will receive the PV
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param undo_stack    pre-allocated stack for undo information
 * \param stats         structure for tracking search stats
 * 
 * \return the score
 */
int32_t search(position_t* pos, move_line_t* parent_pv, int32_t depth, 
    int32_t alpha, int32_t beta, move_t* move_stack, undo_t* undo_stack, 
    stats_t* stats)
{
    /* initialize the move line structure */
    memset(parent_pv, 0, sizeof(move_line_t));

    /* initialize the stats structure */
    memset(stats, 0, sizeof(stats_t));

    /* initialize the killer move arrays */
    memset(killer1, 0, sizeof(move_t) * MAX_PLY);
    memset(killer2, 0, sizeof(move_t) * MAX_PLY);


    return search_helper(pos, parent_pv, 0, depth, alpha, beta, move_stack, 
        undo_stack, stats);
}


static int32_t search_helper(position_t* pos, move_line_t* parent_pv, int ply, 
    int32_t depth, int32_t alpha, int32_t beta, move_t* move_stack, 
    undo_t* undo_stack, stats_t* stats)
{
    assert (depth >= 0);

    stats->nodes++;
    parent_pv->n = 0;

    /* base case */
    if (depth == 0)
    {
        return eval(pos, false);
    }

    /* draw check */
    if (is_draw(pos, undo_stack))
    {
        stats->draws++;
        return 0;
    }

    /* prepare to search */
    int num_moves_searched = 0;
    move_line_t pv; pv.n = 0;

    move_order_dto mo_dto;
    initialize_move_ordering(&mo_dto, move_stack, NO_MOVE, killer1[ply], killer2[ply]);

    move_t* mp;
    undo_t* uptr = undo_stack + pos->move_counter;
    while (next(pos, &mp, &mo_dto))
    {
        apply_move(pos, *mp, uptr);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, uptr);
            continue;
        }

        int32_t score = -search_helper(
            pos, &pv, ply+1, depth-1, -beta, -alpha, mo_dto.end, undo_stack, 
            stats);
        ++num_moves_searched;

        undo_move(pos, uptr);

        if (score >= beta)
        {
            stats->fail_highs++;
            if (!is_capture(*mp) && !get_promopiece(*mp))
            {
                add_killer(*mp, ply);
            }
            return beta;
        }
        if (score > alpha)
        {
            alpha = score;
            set_parent_pv(parent_pv, *mp, &pv);
        }
    }

    alpha = adjust_score_for_mate(pos, alpha, num_moves_searched, ply);

    return alpha;
}


static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply)
{
    int32_t adjusted_score = score;

    if (num_moves_searched == 0)
    {
        if (in_check(pos, pos->player))
        {
            adjusted_score = -CHECKMATE + ply;
        }
        else
        {
            /* draw score */
            adjusted_score = 0;
        }
    }

    return adjusted_score;
}

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail)
{
    assert(head != NO_MOVE);

    /* set the head of the PV, without any scoring fields */
    parent_pv->mv[0] = clear_score(head);

    /* set the tail of the PV */
    memcpy(parent_pv->mv + 1, tail->mv, tail->n * sizeof(move_t));

    /* set the size of the line */
    parent_pv->n = tail->n + 1;
}

static void add_killer(move_t killer_move, int ply)
{
    if (killer_move != killer1[ply])
    {
        if (killer_move != killer2[ply]) 
        {
            /* shift */
            killer2[ply] = killer1[ply];
            killer1[ply] = killer_move;
        }
        else
        {
            /* swap them */
            move_t tmp = killer1[ply];
            killer1[ply] = killer2[ply];
            killer2[ply] = tmp;
        }
    }
}

static bool is_draw(const position_t* pos, const undo_t* undo_stack)
{
    return pos->fifty_counter >= 100 
        || is_lack_of_mating_material(pos)
        || is_draw_rep(pos, undo_stack);
}
