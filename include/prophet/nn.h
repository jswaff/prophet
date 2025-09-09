#pragma once

#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif


/**
 * \brief Load neural network weights file
 * 
 * \return 0 on successful execution, and non-zero on failure
 */
int load_neural_network(const char* weights_file);


/**
 * \brief Evaluate a chess position for the side to move using a neural network.
 *
 * Run a forward pass (inference) using the previously loaded neural network.
 * 
 * \param fen             a pointer to a chess position
 *
 * \return the score.
 */
int32_t nn_eval_from_fen(const char* fen);


/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif
