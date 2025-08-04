#include "nn_internal.h"

#include "prophet/position.h"

#include "bitmap/bitmap.h"
#include "position/square_internal.h"

#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

/* forward decls */
static void nn_add_pieces(int piece_type, int piece_color, uint64_t piece_map, const neural_network_t* nn,
                          nnue_accumulator_t* acc);
#if !defined(USE_AVX) || defined(DEBUG_AVX)
static void nn_move_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                      int from_feature_w, int to_feature_w,
                                      int from_feature_b, int to_feature_b);
static void nn_add_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                     int feature_w, int feature_b);
static void nn_remove_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                        int feature_w, int feature_b);
#endif
static inline int get_nnue_piece_type(int piece_type);

/**
 * \brief Fully populate the NNUE accumulators in a chess position.
 * 
 * \param pos             a pointer to a chess position
 * \param nn              a pointer to a neural network model
 */
void populate_accumulators(position_t* pos, const neural_network_t* nn) {

    /* initialize with bias weights */
    for (int i=0;i<NN_SIZE_L1;i++) {
        pos->nnue_accumulator[0][i] = nn->B0[i];
        pos->nnue_accumulator[1][i] = nn->B0[i];
    }

    /* add pieces */
    nn_add_pieces(ROOK, WHITE, pos->white_rooks, nn, &pos->nnue_accumulator);
    nn_add_pieces(ROOK, BLACK, pos->black_rooks, nn, &pos->nnue_accumulator);
    nn_add_pieces(KNIGHT, WHITE, pos->white_knights, nn, &pos->nnue_accumulator);
    nn_add_pieces(KNIGHT, BLACK, pos->black_knights, nn, &pos->nnue_accumulator);
    nn_add_pieces(BISHOP, WHITE, pos->white_bishops, nn, &pos->nnue_accumulator);
    nn_add_pieces(BISHOP, BLACK, pos->black_bishops, nn, &pos->nnue_accumulator);
    nn_add_pieces(QUEEN, WHITE, pos->white_queens, nn, &pos->nnue_accumulator);
    nn_add_pieces(QUEEN, BLACK, pos->black_queens, nn, &pos->nnue_accumulator);
    nn_add_pieces(PAWN, WHITE, pos->white_pawns, nn, &pos->nnue_accumulator);
    nn_add_pieces(PAWN, BLACK, pos->black_pawns, nn, &pos->nnue_accumulator);
    nn_add_piece(KING, WHITE, pos->white_king, nn, &pos->nnue_accumulator);
    nn_add_piece(KING, BLACK, pos->black_king, nn, &pos->nnue_accumulator);
}

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
        nnue_accumulator_t* acc)
{
    int nnue_piece_type = get_nnue_piece_type(piece);
    int nnue_piece_color = !piece_color;

    int index_w = nnue_piece_type * 2 + nnue_piece_color;
    int from_feature_w = (64 * index_w) + from;
    int to_feature_w = (64 * index_w) + to;

    int index_b = nnue_piece_type * 2 + (1 - nnue_piece_color);
    int from_feature_b = (64 * index_b) + (from ^ 56);
    int to_feature_b = (64 * index_b) + (to ^ 56);

#ifdef USE_AVX
#ifdef DEBUG_AVX
    nnue_accumulator_t my_acc;
    memcpy(&my_acc, acc, sizeof(nnue_accumulator_t));
#endif
    __m256i acc0, wei;
    /* white POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[0][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * from_feature_w + i]);
        acc0 = _mm256_sub_epi16(acc0, wei);
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * to_feature_w + i]);
        acc0 = _mm256_add_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[0][i]), acc0);
    }
    /* black POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[1][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * from_feature_b + i]);
        acc0 = _mm256_sub_epi16(acc0, wei);
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * to_feature_b + i]);
        acc0 = _mm256_add_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[1][i]), acc0);
    }

#ifdef DEBUG_AVX
    nn_move_piece_helper_slow(nn, &my_acc, from_feature_w, to_feature_w, from_feature_b, to_feature_b);
    assert(accumulators_equal(&my_acc, acc));
#endif /* DEBUG_AVX */
#else /* without AVX intrinsics */
    nn_move_piece_helper_slow(nn, acc, from_feature_w, to_feature_w, from_feature_b, to_feature_b);
#endif /* USE_AVX */

}

/**
 * \brief Incrementally update the NNUE accumulators when a piece is being added.
 * 
 * \param piece           the piece type
 * \param piece_color     the color of the moving piece
 * \param sq              the square the piece is being added to
 * \param nn              a pointer to a neural network model
 * \param acc             a pointer to the NNUE accumulators to update
 */
void nn_add_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc) {

    int nnue_piece_type = get_nnue_piece_type(piece);
    int nnue_piece_color = !piece_color;

    int index_w = nnue_piece_type * 2 + nnue_piece_color;
    int feature_w = (64 * index_w) + sq;

    int index_b = nnue_piece_type * 2 + (1 - nnue_piece_color);
    int feature_b = (64 * index_b) + (sq ^ 56);

#ifdef USE_AVX
#ifdef DEBUG_AVX
    nnue_accumulator_t my_acc;
    memcpy(&my_acc, acc, sizeof(nnue_accumulator_t));
#endif
    __m256i acc0, wei;
    /* white POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[0][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * feature_w + i]);
        acc0 = _mm256_add_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[0][i]), acc0);
    }
    /* black POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[1][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * feature_b + i]);
        acc0 = _mm256_add_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[1][i]), acc0);
    }
#ifdef DEBUG_AVX
    nn_add_piece_helper_slow(nn, &my_acc, feature_w, feature_b);
    assert(accumulators_equal(&my_acc, acc));
#endif /* DEBUG_AVX */
#else /* without AVX intrinsics */
    nn_add_piece_helper_slow(nn, acc, feature_w, feature_b);
#endif /* USE_AVX */

}

/**
 * \brief Incrementally update the NNUE accumulators when a piece is being removed.
 * 
 * \param piece           the piece type
 * \param piece_color     the color of the moving piece
 * \param sq              the square the piece is being removed from
 * \param nn              a pointer to a neural network model
 * \param acc             a pointer to the NNUE accumulators to update
 */
void nn_remove_piece(piece_t piece, color_t piece_color, square_t sq, const neural_network_t* nn, nnue_accumulator_t* acc)
{
    int nnue_piece_type = get_nnue_piece_type(piece);
    int nnue_piece_color = !piece_color;

    int index_w = nnue_piece_type * 2 + nnue_piece_color;
    int feature_w = (64 * index_w) + sq;

    int index_b = nnue_piece_type * 2 + (1 - nnue_piece_color);
    int feature_b = (64 * index_b) + (sq ^ 56);

#ifdef USE_AVX
#ifdef DEBUG_AVX
    nnue_accumulator_t my_acc;
    memcpy(&my_acc, acc, sizeof(nnue_accumulator_t));
#endif
    __m256i acc0, wei;
    /* white POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[0][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * feature_w + i]);
        acc0 = _mm256_sub_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[0][i]), acc0);
    }
    /* black POV */
    for (int i=0;i<NN_SIZE_L1;i+=16) {
        acc0 = _mm256_loadu_si256((__m256i*) &((*acc)[1][i]));
        wei = _mm256_loadu_si256((__m256i*) &nn->W0[NN_SIZE_L1 * feature_b + i]);
        acc0 = _mm256_sub_epi16(acc0, wei);
        _mm256_storeu_si256((__m256i*) &((*acc)[1][i]), acc0);
    }
#ifdef DEBUG_AVX
    nn_remove_piece_helper_slow(nn, &my_acc, feature_w, feature_b);
    assert(accumulators_equal(&my_acc, acc));
#endif /* DEBUG_AVX */
#else /* without AVX intrinsics */
    nn_remove_piece_helper_slow(nn, acc, feature_w, feature_b);
#endif /* USE_AVX */
}

/**
 * \brief Utility method to testing if two accumulators are equal.
 * 
 * \param acc1            a pointer to the first accumulator
 * \param acc2            a pointer to the second accumulator
 * 
 * \return true if the accumulators are equal, otherwise false if they are not
 */
bool accumulators_equal(const nnue_accumulator_t* acc1, const nnue_accumulator_t* acc2) {
    for (int i=0;i<2;i++) {
        for (int j=0;j<NN_SIZE_L1;j++) {
            if ((*acc1)[i][j] != (*acc2)[i][j]) return false;
        }
    }
    return true;
}

static void nn_add_pieces(int piece_type, int piece_color, uint64_t piece_map, const neural_network_t* nn,
    nnue_accumulator_t* acc) 
{
    while (piece_map) {
        square_t sq = (square_t)get_lsb(piece_map);
        nn_add_piece(piece_type, piece_color, sq, nn, acc);
        piece_map ^= square_to_bitmap(sq);
    }
}

#if !defined(USE_AVX) || defined(DEBUG_AVX)
static void nn_move_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                      int from_feature_w, int to_feature_w,
                                      int from_feature_b, int to_feature_b)
{
    for (int i=0;i<NN_SIZE_L1;i++) {
        (*acc)[0][i] -= nn->W0[NN_SIZE_L1 * from_feature_w + i];
        (*acc)[0][i] += nn->W0[NN_SIZE_L1 * to_feature_w + i];
        (*acc)[1][i] -= nn->W0[NN_SIZE_L1 * from_feature_b + i];
        (*acc)[1][i] += nn->W0[NN_SIZE_L1 * to_feature_b + i];
    }
}

static void nn_add_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                     int feature_w, int feature_b)
{
    for (int i=0;i<NN_SIZE_L1;i++) {
        (*acc)[0][i] += nn->W0[NN_SIZE_L1 * feature_w + i];
        (*acc)[1][i] += nn->W0[NN_SIZE_L1 * feature_b + i];
    }
}

static void nn_remove_piece_helper_slow(const neural_network_t* nn, nnue_accumulator_t* acc,
                                        int feature_w, int feature_b)
{
    for (int i=0;i<NN_SIZE_L1;i++) {
        (*acc)[0][i] -= nn->W0[NN_SIZE_L1 * feature_w + i];
        (*acc)[1][i] -= nn->W0[NN_SIZE_L1 * feature_b + i];
    }
}
#endif

static inline int get_nnue_piece_type(int piece_type) {
    return abs(piece_type) - 1;
}
