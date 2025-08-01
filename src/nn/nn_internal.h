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

/**
 * \brief Fully populate the NNUE accumulators in a chess position.
 * 
 * \param pos             a pointer to a chess position
 * \param nn              a pointer to a neural network model
 */
void populate_accumulators(position_t* pos, const neural_network_t *nn);


/**
 * \brief Incrementally update the NNUE accumulators when a piece is moved.
 * 
 * This is equivalent to calling nn_remove_piece(from) followed by nn_add_piece(to).
 * 
 * \param piece           the piece type
 * \param piece_color     the color of the moving piece
 * \param from            the square the piece is moving from
 * \param to              the square the piece is moving to
 * \param nn              a pointer to a neural network model
 * \param acc             a pointer to the NNUE accumulators to update
 */
void nn_move_piece(piece_t piece, color_t piece_color, square_t from, square_t to, const neural_network_t* nn,
                   nnue_accumulator_t* acc);

/**
 * \brief Incrementally update the NNUE accumulators when a piece is being added.
 * 
 * \param piece           the piece type
 * \param piece_color     the color of the moving piece
 * \param sq              the square the piece is being added to
 * \param nn              a pointer to a neural network model
 * \param acc             a pointer to the NNUE accumulators to update
 */
void nn_add_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc);


/**
 * \brief Incrementally update the NNUE accumulators when a piece is being removed.
 * 
 * \param piece           the piece type
 * \param piece_color     the color of the moving piece
 * \param sq              the square the piece is being removed from
 * \param nn              a pointer to a neural network model
 * \param acc             a pointer to the NNUE accumulators to update
 */
void nn_remove_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc);


/**
 * \brief Utility method to testing if two accumulators are equal.
 * 
 * \param acc1            a pointer to the first accumulator
 * \param acc2            a pointer to the second accumulator
 * 
 * \return true if the accumulators are equal, otherwise false if they are not
 */
bool accumulators_equal(const nnue_accumulator_t* acc1, const nnue_accumulator_t* acc2);


// make this header C++ friendly.
#ifdef     __cplusplus
}
#endif    //__cplusplus
