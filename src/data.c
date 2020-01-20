#include <prophet/const.h>
#include <prophet/hash.h>
#include <prophet/position/position.h>

/* the game position, or global position */
position_t gpos;

/* move stack */
move_t gmoves[MAX_PLY * MAX_MOVES_PER_PLY];

/* undo information for the game position*/
undo_t gundos[MAX_HALF_MOVES_PER_GAME];

zobrist_keys zkeys;
