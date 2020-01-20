#include <prophet/const.h>
#include <prophet/eval.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/search.h>

#include  <assert.h>
#include <string.h>

static int32_t adjust_score_for_mate(const position_t* pos, int32_t score, 
    int num_moves_searched, int ply);

static int32_t search_helper(position_t* pos, int ply, int32_t depth, 
    int32_t alpha, int32_t beta, stats_t* stats);


/**
 * \brief Search the position to a fixed depth.
 *
 * \param pos           a pointer to a chess position
 * \param depth         the depth to search to
 * \param alpha         the lower bound
 * \param beta          the upper bound
 * \param stats         structure for tracking search stats
 * 
 * \return the score
 */
int32_t search(position_t* pos, int32_t depth, int32_t alpha, int32_t beta,
    stats_t* stats)
{
    /* initialize the stats structure */
    memset(stats, 0, sizeof(stats_t));

    return search_helper(pos, 0, depth, alpha, beta, stats);
}


static int32_t search_helper(position_t* pos, int ply, int32_t depth, 
    int32_t alpha, int32_t beta, stats_t* stats)
{
    assert (depth >= 0);

    stats->nodes++;

    if (depth == 0)
    {
        return eval(pos, false);
    }

    move_t moves[300];
    move_t* endp = gen_pseudo_legal_moves(moves, pos, true, true);

    move_t* mp;
    int num_moves_searched = 0;
    for (mp = moves; mp < endp; mp++)
    {
        undo_t u;
        apply_move(pos, *mp, &u);

        /* verify the move was legal */
        if (in_check(pos, opposite_player(pos->player)))
        {
            undo_move(pos, &u);
            continue;
        }

        int32_t score = -search_helper(pos, ply+1, depth-1, -beta, -alpha, 
            stats);
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