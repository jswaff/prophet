#include "xboard_internal.h"

#include "prophet/const.h"
#include "prophet/movegen.h"
#include "prophet/position.h"

#include "../../util/output.h"

#include <stdbool.h>

extern position_t gpos;
extern undo_t gundos[MAX_HALF_MOVES_PER_GAME];


/**
 * \brief Check for an end-of-game condition and print RESULT if appropriate.
 *
 * \return true if the game is over, false otherwise.
 */
bool endgame_check()
{
    char* result = 0;

    /* if the game is over by rule, print the result */
    if (is_checkmate(&gpos)) {
        if (gpos.player == WHITE) {
            result = "0-1 {Black mates}";
        } else {
            result = "1-0 {White mates}";
        }
    } else if (is_stalemate(&gpos)) {
        result = "1/2-1/2 {Stalemate}";
    } else if (is_lack_of_mating_material(&gpos)) {
        result = "1/2-1/2 {Insufficient material}";
    } else if (is_draw50(&gpos)) {
        result = "1/2-1/2 {50 move rule}";
    } else if (is_draw_rep(&gpos, gundos, 2)) {
        result = "1/2-1/2 {Draw by repetition}";
    }

    if (result != 0) {
        plog("%s\n", result);
        return true;
    }

    return false;
}
