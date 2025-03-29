#include "nn_internal.h"

#include "prophet/position.h"

#include <math.h>
#include <stdint.h>

static int clamp(int val, int min, int max);
static int my_round(float val);

/**
 * \brief Evaluate a chess position for the side to move using a neural network.
 *
 * Run a forward pass (inference) using the supplied neural network.
 * 
 * \param pos             a pointer to a chess position
 * \param nn              a pointer to a neural network model
 *
 * \return the score.
 */
int nn_eval(const position_t* pos, const neural_network_t* nn) {

    /* set layer 1 from accumulators */
    int8_t L1[NN_SIZE_L1 * 2];
    for (int i=0;i<NN_SIZE_L1;i++) {
        L1[i] = clamp(pos->nnue_accumulator[0][i], 0, THRESHOLD);
        L1[NN_SIZE_L1 + i] = clamp(pos->nnue_accumulator[1][i], 0, THRESHOLD);
    }

    /* calculate layer 2 */
    int32_t L2[NN_SIZE_L2];
    for (int i=0;i<NN_SIZE_L2;i++) {
        int32_t sum = nn->B1[i];
        for (int j=0;j<NN_SIZE_L1*2;j++) {
            sum += nn->W1[i * (NN_SIZE_L1 * 2) + j]  * L1[j];
        }
        L2[i] = sum;
    }

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

static int my_round(float val) {
    if (val > 0) return (int)(val + 0.5);
    else return (int)(val - 0.5);
}
