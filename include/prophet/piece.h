#pragma once

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

typedef enum { NO_PIECE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING } piece_t;


static inline bool is_white_piece(int32_t piece) 
{
    return piece > NO_PIECE;
}


static inline bool is_black_piece(int32_t piece) 
{
    return piece < NO_PIECE;
}


static inline bool is_not_white_piece(int32_t piece) 
{
    return piece <= NO_PIECE;
}


static inline bool is_not_black_piece(int32_t piece) 
{
    return piece >= NO_PIECE;
}


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif 
