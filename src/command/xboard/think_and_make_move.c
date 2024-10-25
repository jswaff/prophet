#include "xboard_internal.h"

#include "prophet/const.h"
#include "prophet/error_codes.h"
#include "prophet/movegen.h"
#include "prophet/parameters.h"
#include "prophet/search.h"

#include "../../util/output.h"
#include "../../util/string_utils.h"

#include <assert.h>
#include <pthread.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>


/* externs */
extern bool fixed_time_per_move;
extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];
extern volatile uint32_t max_depth;
extern volatile uint32_t max_time_ms;
extern bool xboard_post_mode;
extern volatile bool stop_search;
extern bool xboard_force_mode;


/* local variables */
uint32_t num_random_starting_moves;
uint32_t random_moves_counter;
pthread_t search_thread;
bool search_thread_running = false;
pthread_mutex_t search_lock;
move_t moves[MAX_PLY * MAX_MOVES_PER_PLY];


/* forward decls */
static void* iterate_wrapper(void* arg);
static int select_random_move();
static int make_move_otb(move_t mv);


/**
 * \brief Start the engine "thinking"  and (eventually) make a move.
 *
 * If the engine is in random mode, a move will be made immediately.
 * Otherwise, a separate thread will be started and the method will return.
 * Once the thread terminates, the move will be applied to the global
 * position.
 *
 * If the selected move results in the end of the game, the game
 * result is displayed.
 *
 * This method should not be called if the game is already over.
 *
 * \return 0 on successful execution, and non-zero on failure
 *
 */
int think_and_make_move()
{
    assert(!endgame_check());

    int retval;
    if (random_moves_counter > 0) {
        plog("# selecting random move.  random_moves_counter: %d\n", random_moves_counter);
        retval = select_random_move();
        random_moves_counter--;
    } else {
        pthread_mutex_lock(&search_lock); 
        stop_search = false;
        retval = pthread_create(&search_thread, NULL, iterate_wrapper, NULL);
        if (0 != retval) {
            return P4_ERROR_THREAD_CREATION_FAILURE;
        }
        search_thread_running = true;
        pthread_mutex_unlock(&search_lock); 
    }


    return retval;
}


static void* iterate_wrapper(void* UNUSED(arg))
{
    iterator_options_t* opts = (iterator_options_t*)malloc(sizeof(iterator_options_t));
    memset(opts, 0, sizeof(iterator_options_t));
    opts->early_exit_ok = !fixed_time_per_move;
    opts->max_depth = max_depth;
    opts->max_time_ms = max_time_ms;
    opts->post_mode = xboard_post_mode;
    opts->clear_hash_each_search = false;

    iterator_context_t* ctx = (iterator_context_t*)malloc(sizeof(iterator_context_t));
    ctx->pos = (position_t*)malloc(sizeof(position_t));
    memcpy(ctx->pos, &gpos, sizeof(position_t));
    ctx->move_stack = moves;
    ctx->undo_stack = gundos;

    move_line_t pv = iterate(opts, ctx);

    free(ctx->pos);
    free(ctx);
    free(opts);

    if (!xboard_force_mode) {
        int retval = make_move_otb(pv.mv[0]);
        if (0 != retval) {
            pthread_exit(&retval);
        }
    }

    return 0;
}


/**
 * \brief Choose a move randomly.
 *
 * Choose a random move from the global chess position and make the move.
 *
 * \return 0 if successful, non-zero on error.
 */
static int select_random_move()
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
    for (const move_t* mp = moves; mp < endp; mp++) {
        if (*mp != NO_MOVE) {
            if (i == mv_ind) {
                return make_move_otb(*mp);
            }
            i++;
        }
    }

    /* we should never get here, but the compiler doesn't know that. */
    assert(false);
    return 0;
}


/**
 * \brief Make a move over the global game board.
 *
 * \param mv           The move to apply
 *
 * \return 0 if successful, non-zero on error.
 */
static int make_move_otb(move_t mv)
{
    if (gpos.move_counter >= MAX_HALF_MOVES_PER_GAME) {
        return P4_ERROR_GUNDO_INDEX_UB_VIOLATION;
    }

    apply_move(&gpos, mv, gundos + gpos.move_counter);
    char* str_engine_mv = move_to_str(mv);
    plog("move %s\n", str_engine_mv);
    free(str_engine_mv);
    endgame_check();

    return 0;
}
