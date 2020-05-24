#include <prophet/parameters.h>
#include <prophet/position/position.h>

#include <stdlib.h>
#include <pthread.h>
#include <time.h>

bool initialized = false;

extern void init_bitmap_lsb_msb();
extern void init_bitmap_file_rank();
extern void init_bitmap_ray();
extern void init_squares();
extern void init_zkeys();

extern void init_king_movegen();
extern void init_knight_movegen();
extern void init_bishop_movegen();
extern void init_rook_movegen();

extern void init_pawn_isolated();
extern void init_pawn_passed();

extern position_t gpos;
extern pthread_t search_thread;

static void* init_helper(void *ptr);

/**
 * \brief Initialize the program.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int init()
{
    if (!initialized) 
    {
        pthread_create(&search_thread, NULL, init_helper, NULL);

        /* initialize the random number generator */
        time_t t;
        srand((unsigned) time(&t));

        init_bitmap_lsb_msb();
        init_bitmap_file_rank();
        init_squares();
        init_zkeys();
        init_bishop_movegen();
        init_king_movegen();
        init_knight_movegen();
        init_rook_movegen();
        init_pawn_isolated();
        init_bitmap_ray();
        init_pawn_passed();
        
        reset_pos(&gpos);
    }

    initialized = true;

    return 0;
}

/* silly hack to init search_thread */ 
static void* init_helper(void *UNUSED(ptr)) { return 0; }
