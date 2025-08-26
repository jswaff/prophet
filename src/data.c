#include "prophet/const.h"
#include "prophet/position.h"

#include "hash/hash_internal.h"

/* the game position, or global position */
position_t gpos;

/* killer moves */
move_t killer1[MAX_PLY];
move_t killer2[MAX_PLY];

/* undo information for the game position*/
undo_t gundos[MAX_HALF_MOVES_PER_GAME];

zobrist_keys zkeys;
