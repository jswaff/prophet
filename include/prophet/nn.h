#pragma once

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif

#define NN_SIZE_L1 1536
#define NN_SIZE_L2    1

typedef int16_t nnue_accumulator_t[2][NN_SIZE_L1];

typedef struct {
    int8_t W0[768 * NN_SIZE_L1];
    int8_t B0[NN_SIZE_L1];
    int8_t W1[NN_SIZE_L1 * 2 * NN_SIZE_L2];
    int8_t B1[NN_SIZE_L2];
} neural_network_t;

/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif
