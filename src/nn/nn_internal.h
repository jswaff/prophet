#pragma once

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  

#define NN_SIZE_L1 1536
#define NN_SIZE_L2    1

static const unsigned int SCALE = 64;
static const unsigned int THRESHOLD = 127;

typedef struct {

    int8_t W0[768 * NN_SIZE_L1];
    int8_t B0[NN_SIZE_L1];
    int8_t W1[NN_SIZE_L1 * 2 * NN_SIZE_L2];
    int8_t B1[NN_SIZE_L2];

} neural_network_t;


int load_neural_network(neural_network_t* nn, const char* weights_file);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
