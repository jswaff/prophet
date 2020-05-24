#include <prophet/const.h>
#include <prophet/error_codes.h>
#include <prophet/movegen.h>
#include <prophet/parameters.h>
#include <prophet/search.h>
#include <prophet/util/output.h>
#include <prophet/util/string_utils.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "xboard_internal.h"

/* externs */
extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern int32_t max_depth;
extern bool xboard_post_mode;

/* local variables */
bool random_mode = false;
pthread_t search_thread;
move_t moves[MAX_PLY * MAX_MOVES_PER_PLY];


/* forward decls */
static void* iterate_wrapper(void* arg);
//static move_t select_random_move();


/**
 * \brief Select a move, and apply it to the game position.  If the game is
 * over, print the result.
 *
 * This method should not be called if the game is already over.
 *
 * \return 0 on successful execution, and non-zero on failure
 *
 */
int think_and_make_move()
{
    /* TODO - perhaps a join or exit statement */

    assert(!endgame_check());

    // if (random_mode)
    // {
    //     engine_mv = select_random_move();
    // }
    // else
    // {
        // engine_mv = select_move();
    // }


    /* search the position to a fixed depth */
    int retval = pthread_create(&search_thread, NULL, iterate_wrapper, NULL);
    if (retval != 0)
    {
        /* TODO: translate error if retval != 0 */
    }

    return retval;
}


void stop_search_thread_blocking()
{
    pthread_join(search_thread, NULL);
}


static void* iterate_wrapper(void* UNUSED(arg))
{
    iterator_options_t* opts = (iterator_options_t*)malloc(sizeof(iterator_options_t));
    memset(opts, 0, sizeof(iterator_options_t));
    opts->early_exit_ok = true;
    opts->max_depth = max_depth;
    opts->post_mode = xboard_post_mode;

    iterator_context_t* ctx = (iterator_context_t*)malloc(sizeof(iterator_context_t));
    ctx->pos = (position_t*)malloc(sizeof(position_t));
    memcpy(ctx->pos, &gpos, sizeof(position_t));
    ctx->move_stack = moves;
    ctx->undo_stack = gundos;

    move_line_t pv = iterate(opts, ctx);

    free(ctx->pos);
    free(ctx);
    free(opts);

    /* apply the move to the global position */
    // if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME)
    // {
    //     return P4_ERROR_GUNDO_INDEX_UB_VIOLATION;
    // }

    apply_move(&gpos, pv.mv[0], gundos + gpos.move_counter);
    char* str_engine_mv = move_to_str(pv.mv[0]);
    out(stdout, "move %s\n", str_engine_mv);
    free(str_engine_mv);

    endgame_check();

    return 0;
}


/**
 * \brief Choose a move randomly.
 *
 * Choose a random move from the global chess position.
 *
 * \return a move.
 */
#if 0
static move_t select_random_move()
{
    /* generate legal moves */
    move_t *endp = gen_legal_moves(moves, &gpos, true, true);

    /* count the number of moves to choose from */
    int num_caps, num_noncaps;
    num_moves_in_list(moves, endp, &num_caps, &num_noncaps);
    int num_moves = num_caps + num_noncaps;
    assert(num_moves > 0);

    /* choose a random move. */
    int mv_ind = rand() % num_moves;

    /* fetch the move from the stack.  since the list isn't contiguous, (it
     * contains some NO_MOVE entries), so we need to iterate. */
    int i = 0;
    for (const move_t* mp = moves; mp < endp; mp++) 
    {
        if (*mp != NO_MOVE)
        {
            if (i == mv_ind)
            {
                return *mp;
            }
            i++;
        }
    }

    /* we should never get here, but the compiler doesn't know that. */
    assert(false);
    return NO_MOVE;
}
#endif