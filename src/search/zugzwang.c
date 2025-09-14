#include "search_internal.h"

#include "position/position.h"

#include <stdbool.h>


bool zugzwang(const position_t *pos) 
{
    if (pos->piece_counts[WHITE][ROOK] + pos->piece_counts[WHITE][KNIGHT]
           + pos->piece_counts[WHITE][BISHOP] + pos->piece_counts[WHITE][QUEEN] == 0)
    {
        return true;
    }

    if (pos->piece_counts[BLACK][ROOK] + pos->piece_counts[BLACK][KNIGHT]
           + pos->piece_counts[BLACK][BISHOP] + pos->piece_counts[BLACK][QUEEN] == 0)
    {
        return true;
    }


    return false;
}

