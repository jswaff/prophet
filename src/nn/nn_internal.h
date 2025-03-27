#pragma once

#include "prophet/nn.h"
#include "prophet/position.h"

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif  


static const unsigned int SCALE = 64;
static const unsigned int THRESHOLD = 127;

void populate_accumulators(position_t* pos, const neural_network_t *nn);
void nn_move_piece(piece_t piece, color_t piece_color, square_t from, square_t to, const neural_network_t* nn,
                   nnue_accumulator_t* acc);
void nn_add_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc);
void nn_remove_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc);

bool accumulators_equal(const nnue_accumulator_t* acc1, const nnue_accumulator_t* acc2);

int nn_eval(const position_t* pos, const neural_network_t *nn);

// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
