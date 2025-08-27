#pragma once

#include <prophet/const.h>
#include <prophet/position.h>

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif


typedef struct {
    int16_t W0[768 * NN_SIZE_L1];
    int16_t B0[NN_SIZE_L1];
    int8_t W1[NN_SIZE_L1 * 2 * NN_SIZE_L2];
    int8_t B1[NN_SIZE_L2];
} neural_network_t;


/**
 * \brief Evaluate a chess position for the side to move using a neural network.
 *
 * Run a forward pass (inference) using the previously loaded neural network.
 * 
 * \param fen             a pointer to a chess position
 *
 * \return the score.
 */
int nn_eval_from_fen(const char *fen);


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
int nn_eval(const position_t* pos, const neural_network_t *nn);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif
