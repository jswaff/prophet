#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/search.h>

#include <assert.h>
#include <string.h>

static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply);

static int32_t search_helper(position_t* pos, move_line_t* parent_pv, int ply, 
    int32_t depth, int32_t alpha, int32_t beta, move_t* move_stack, 
    stats_t* stats);

static void set_parent_pv(move_line_t* parent_pv, const move_t head, 
    const move_line_t* tail);


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param parent_pv     a pointer to the move line that will receive the PV
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param move_stack    pre-allocated stack for move generation
 * \param stats         structure for tracking search stats
 * 
 * \return the score
 */
int32_t search(position_t* pos, move_line_t* parent_pv, int32_t depth, 
    int32_t alpha, int32_t beta, move_t* move_stack, stats_t* stats)
{
    /* initialize the move line structure */
    memset(parent_pv, 0, sizeof(move_line_t));

    /* initialize the stats structure */
    memset(stats, 0, sizeof(stats_t));

    return search_helper(pos, parent_pv, 0, depth, alpha, beta, move_stack, 
        stats);
}


static int32_t search_helper(position_t* pos, move_line_t* parent_pv, int ply, 
    int32_t depth, int32_t alpha, int32_t beta, move_t* move_stack, 
    stats_t* stats)
{
    assert (depth >= 0);

    stats->nodes++;
    parent_pv->n = 0;

    /* base case */
    if (depth == 0)
    {
        return eval(pos, false);
    }


    /* prepare to search */
    move_t* endp = gen_pseudo_legal_moves(move_stack, pos, true, true);
    move_t* mp;
    int num_moves_searched = 0;
    move_line_t pv; pv.n = 0;

    for (mp = move_stack; mp < endp; mp++)
    {
        undo_t u;
        apply_move(pos, *mp, &u);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, &u);
            continue;
        }

        int32_t score = -search_helper(
            pos, &pv, ply+1, depth-1, -beta, -alpha, endp, stats);
        ++num_moves_searched;

        undo_move(pos, &u);

        if (score >= beta)
        {
            stats->fail_highs++;
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
