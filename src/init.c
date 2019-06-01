#include <stdbool.h>

bool initialized = false;

extern void init_bitmaps();
extern void init_squares();
extern void init_zkeys();

extern void init_king_movegen();
extern void init_knight_movegen();
extern void init_bishop_movegen();
extern void init_rook_movegen();

void init()
{
    if (!initialized) {
        init_bitmaps();
        init_squares();
        init_zkeys();
        init_bishop_movegen();
        init_king_movegen();
        init_knight_movegen();
        init_rook_movegen();
    }

    initialized = true;
}
