#pragma once

#include <prophet/move.h>

#include <stdbool.h>

// make this header C++ friendly.
#ifdef     __cplusplus
extern "C" {
#endif    //__cplusplus

void generate_moves_from_fen(move_t* m, int* n, const char* fen, bool caps, bool noncaps);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus

