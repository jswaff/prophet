#pragma once

#include "hash/hash_internal.h"
#include "nn/nn_internal.h"
#include "position/position.h"
#include "prophet/move.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern position_t gpos;
extern bool xboard_force_mode;
extern bool xboard_post_mode;
extern bool fixed_time_per_move;
extern bool use_neural_network;
extern volatile bool stop_search;
extern uint32_t time_control_moves;
extern volatile uint32_t max_time_ms;
extern volatile uint32_t max_depth;
extern volatile uint32_t hash_age;
extern uint32_t time_remaining_millis;
extern double time_control_increment;
extern char time_control_base[10];
extern hash_table_t htbl;
extern hash_table_t phtbl;
extern neural_network_t neural_network;
extern move_line_t last_pv;

#ifdef __cplusplus
}
#endif
