#pragma once

#include "prophet/position.h"

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


static const unsigned int SCALE = 64;
static const unsigned int THRESHOLD = 127;

typedef struct {

    int8_t W0[768 * NN_SIZE_L1];
    int8_t B0[NN_SIZE_L1];
    int8_t W1[NN_SIZE_L1 * 2 * NN_SIZE_L2];
    int8_t B1[NN_SIZE_L2];

} neural_network_t;


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
