#ifndef _PIECE_H_
#define _PIECE_H_

#include <stdbool.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus


enum piece_t { NO_PIECE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
typedef enum piece_t piece_t;


static inline bool is_white_piece(int32_t piece) {
    return piece > NO_PIECE;
}

static inline bool is_black_piece(int32_t piece) {
    return piece < NO_PIECE;
}

static inline bool is_not_white_piece(int32_t piece) {
    return piece <= NO_PIECE;
}

static inline bool is_not_black_piece(int32_t piece) {
    return piece >= NO_PIECE;
}


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

#endif /* ! defined _PIECE_H_ */
