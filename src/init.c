#include <stdbool.h>

#include <prophet/position/position.h>

bool initialized = false;

extern void init_bitmaps();
extern void init_squares();
extern void init_zkeys();

extern void init_king_movegen();
extern void init_knight_movegen();
extern void init_bishop_movegen();
extern void init_rook_movegen();

extern position gpos;

/**
 * \brief Initialize the program.
 *
 * \return 0 on successful execution, and non-zero on failure.
 */
int init()
{
    if (!initialized) 
    {
        init_bitmaps();
        init_squares();
        init_zkeys();
        init_bishop_movegen();
        init_king_movegen();
        init_knight_movegen();
        init_rook_movegen();
        reset_pos(&gpos);
    }

    initialized = true;

    return 0;
}
