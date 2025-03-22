#include "nn_internal.h"

#include "prophet/position.h"

#include <math.h>
#include <stdint.h>

static int clamp(int val, int min, int max);
static void compute_layer(const int8_t* I, const int8_t* W, const int8_t* B, int8_t* O, int I_len, int O_len);
static int my_round(float val);

int nn_eval(const position_t* pos, const neural_network_t* nn) {

    /* set layer 1 from accumulators */
    int8_t L1[NN_SIZE_L1 * 2];
    for (int i=0;i<NN_SIZE_L1;i++) {
        L1[i] = clamp(pos->nnue_accumulator[0][i], 0, THRESHOLD);
        L1[NN_SIZE_L1 + i] = clamp(pos->nnue_accumulator[1][i], 0, THRESHOLD);
    }

    /* calculate layer 2 */
    int8_t L2[NN_SIZE_L2];
    compute_layer(L1, nn->W1, nn->B1, L2, NN_SIZE_L1 * 2, NN_SIZE_L2); 

    /* translate to predicted score */
    float y = ((float)L2[0]) / (SCALE * SCALE);
    int pred = my_round(y * 100); /* centi-pawns */

    return pos->player==WHITE ? pred : -pred;
}

static int clamp(int val, int min, int max) {
    if (val < min) return min;
    if (val >= max) return max;
    return val;
}

static void compute_layer(const int8_t* I, const int8_t* W, const int8_t* B, int8_t* O, int I_len, int O_len) {

    for (int o=0;o<O_len;o++) {
        int8_t sum = B[o];
        for (int i=0;i<I_len;i++) {
            sum += W[o * I_len + i]  * I[i];
        }

        O[o] = sum;
    }
}

static int my_round(float val) {
    return (int)(val + 0.5);
}