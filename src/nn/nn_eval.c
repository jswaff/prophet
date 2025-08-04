#include "prophet/nn.h"

#include "prophet/position.h"

#include "nn_internal.h"

#include <assert.h>
#include <immintrin.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

static int clamp(int val, int min, int max);
static int my_round(float val);
#if !defined(USE_AVX) || defined(DEBUG_AVX)
static void compute_layer2_slow(const neural_network_t* nn, const int8_t* L1, int32_t* L2);
#endif

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

#ifdef USE_AVX
    const __m256i one = _mm256_set1_epi16(1);
    for (int i=0;i<NN_SIZE_L2;i++) {
        __m256i sum0 = _mm256_setzero_si256();
        for (int j=0;j<NN_SIZE_L1*2;j+=32) {
            const __m256i inp = _mm256_loadu_si256((__m256i*) &L1[j]);
            const __m256i wei = _mm256_loadu_si256((__m256i*) &nn->W1[i * (NN_SIZE_L1*2) + j]);            
            const __m256i dot = _mm256_madd_epi16(_mm256_maddubs_epi16(inp, wei), one);
            sum0 = _mm256_add_epi32(sum0, dot);                      
        }

        const __m128i sum128lo = _mm256_castsi256_si128(sum0);
        const __m128i sum128hi = _mm256_extracti128_si256(sum0, 1);
        __m128i sum128 = _mm_add_epi32(sum128lo, sum128hi); 
        sum128 = _mm_add_epi32(sum128, _mm_shuffle_epi32(sum128, _MM_PERM_ABCD));
        sum128 = _mm_add_epi32(sum128, _mm_shuffle_epi32(sum128, _MM_PERM_CDAB));
        L2[i] = _mm_cvtsi128_si32(sum128) + nn->B1[i];
    }

#ifdef DEBUG_AVX
    int32_t L2_debug[NN_SIZE_L2];
    compute_layer2_slow(nn, L1, L2_debug);
    assert(!memcmp(L2, L2_debug, sizeof(int32_t) * NN_SIZE_L2));
#endif /* DEBUG_AVX */
#else /* without AVX intrinsics */
    compute_layer2_slow(nn, L1, L2);
#endif /* USE_AVX */

    /* translate to predicted score */
    float wscore = ((float)L2[0]) / (SCALE * SCALE) * 100; /* to centipawns */
    float wr = ((float)L2[1]) / (SCALE * SCALE) * 1000; /* win ratio */

    int y_hat = my_round((0.5 * wscore)  + (0.5 * wr));

    return pos->player==WHITE ? y_hat : -y_hat;
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

#if !defined(USE_AVX) || defined(DEBUG_AVX)
static void compute_layer2_slow(const neural_network_t* nn, const int8_t* L1, int32_t* L2) {
    for (int i=0;i<NN_SIZE_L2;i++) {
        int32_t sum = nn->B1[i];
        for (int j=0;j<NN_SIZE_L1*2;j++) {
            sum += nn->W1[i * (NN_SIZE_L1 * 2) + j]  * L1[j];
        }
        L2[i] = sum;
    }
}
#endif
