#include "nn_internal.h"

#include "prophet/position.h"

#include "bitmap/bitmap.h"
#include "position/square_internal.h"
#include "parameters.h"


static void add_pieces_to_accumulators(uint64_t piece_map, int piece_type, int piece_color, const neural_network_t* nn, 
    nnue_accumulator_t* acc);
static void add_piece_to_accumulators(int sq, int piece_type, int piece_color, const neural_network_t* nn, nnue_accumulator_t* acc);
static int get_nnue_piece_type(int piece_type);

void populate_accumulators(position_t* pos, const neural_network_t* nn) {

    /* initialize with bias weights */
    for (int i=0;i<NN_SIZE_L1;i++) {
        pos->nnue_accumulator[0][i] = nn->B0[i];
        pos->nnue_accumulator[1][i] = nn->B0[i];
    }

    /* add pieces */
    add_pieces_to_accumulators(pos->white_rooks, ROOK, WHITE, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->black_rooks, ROOK, BLACK, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->white_knights, KNIGHT, WHITE, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->black_knights, KNIGHT, BLACK, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->white_bishops, BISHOP, WHITE, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->black_bishops, BISHOP, BLACK, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->white_queens, QUEEN, WHITE, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->black_queens, QUEEN, BLACK, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->white_pawns, PAWN, WHITE, nn, &pos->nnue_accumulator);
    add_pieces_to_accumulators(pos->black_pawns, PAWN, BLACK, nn, &pos->nnue_accumulator);
    add_piece_to_accumulators(pos->white_king, KING, WHITE, nn, &pos->nnue_accumulator);
    add_piece_to_accumulators(pos->black_king, KING, BLACK, nn, &pos->nnue_accumulator);
}

bool accumulators_equal(const nnue_accumulator_t* UNUSED(acc1), const nnue_accumulator_t* UNUSED(acc2)) {
    return false;
}

static void add_pieces_to_accumulators(uint64_t piece_map, int piece_type, int piece_color, const neural_network_t* nn, 
    nnue_accumulator_t* acc) 
{

    while (piece_map) {
        square_t sq = (square_t)get_lsb(piece_map);
        add_piece_to_accumulators(sq, piece_type, piece_color, nn, acc);    
        piece_map ^= square_to_bitmap(sq);
    }

}

static void add_piece_to_accumulators(int sq, int piece_type, int piece_color, const neural_network_t* nn, nnue_accumulator_t* acc) {

    int nnue_piece_type = get_nnue_piece_type(piece_type);
    int nnue_piece_color = !piece_color;

    int index_w = nnue_piece_type * 2 + nnue_piece_color;
    int feature_w = (64 * index_w) + sq;

    int index_b = nnue_piece_type * 2 + (1 - nnue_piece_color);
    int feature_b = (64 * index_b) + (sq ^ 56);

    for (int i=0;i<NN_SIZE_L1;i++) {
        (*acc)[0][i] += nn->W0[NN_SIZE_L1 * feature_w + i];
        (*acc)[1][i] += nn->W0[NN_SIZE_L1 * feature_b + i];
    }
}

static int get_nnue_piece_type(int piece_type) {
    switch (piece_type) {
        case ROOK: return 0;
        case KNIGHT: return 1;
        case BISHOP: return 2;
        case QUEEN: return 3;
        case KING: return 4;
        default: return 5; // pawn
    }

}
