#pragma once

#include <prophet/const.h>
#include <prophet/move.h>
#include <prophet/piece.h>
#include <prophet/square.h>

#include <stdbool.h>
#include <stdint.h>

/* make this header C++ friendly. */
#ifdef     __cplusplus
extern "C" {
#endif   

typedef enum { BLACK=0, WHITE=1 } color_t;

typedef int16_t nnue_accumulator_t[2][NN_SIZE_L1];

typedef struct {
    int32_t piece[64];
    uint32_t piece_counts[2][7];
    color_t player;
    square_t ep_sq;
    uint32_t castling_rights;
    uint32_t move_counter;
    uint32_t fifty_counter;
    square_t white_king;
    square_t black_king;
    uint64_t white_pawns;
    uint64_t black_pawns;
    uint64_t white_knights;
    uint64_t black_knights;
    uint64_t white_bishops;
    uint64_t black_bishops;
    uint64_t white_rooks;
    uint64_t black_rooks;
    uint64_t white_queens;
    uint64_t black_queens;
    uint64_t white_pieces;
    uint64_t black_pieces;
    uint64_t hash_key;
    uint64_t pawn_key;
    nnue_accumulator_t nnue_accumulator;
} position_t;


typedef struct {
    move_t mv;
    uint64_t hash_key;
    piece_t captured;
    square_t ep_sq;
    uint32_t fifty_counter;
    uint32_t castling_rights;
} undo_t;



/* make this header C++ friendly. */
#ifdef     __cplusplus
}
#endif  
