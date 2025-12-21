# Public API Guide

This document summarizes the public C API exposed under `include/prophet`.
It focuses on common workflows (initialize, evaluate, generate moves, search).

## Build And Link

- Add `include` to your compiler include paths.
- Link against the Prophet library (for example `libprophetlib.a` from the build).

Example compile line (paths are illustrative):

```c
cc -I/path/to/prophet/include my_app.c -L/path/to/prophet/build/lib -lprophetlib
```

## Initialization

Call `init()` once before using search, evaluation, hash tables, or NNUE.

```c
#include <prophet/init.h>

int rc = init();
if (rc != 0) {
    /* handle error */
}
```

## Search API

Header: `include/prophet/search.h`

Use `iterate_from_fen` to search a position and retrieve the principal variation.

```c
#include <prophet/const.h>
#include <prophet/move.h>
#include <prophet/search.h>

stats_t stats;
move_t pv[MAX_PLY];
int pv_len = 0;
uint32_t depth = 0;
int32_t score = 0;

int rc = iterate_from_fen(
    &stats,
    pv,
    &pv_len,
    &depth,
    &score,
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    true,   /* early_exit_ok */
    8,      /* max_depth */
    0,      /* max_time_ms */
    0,      /* max_nodes */
    NULL    /* pv_callback */
);
```

Notes:
- `pv` is populated with up to `MAX_PLY` moves; `pv_len` returns how many.
- Use `set_search_stop_flag(true)` to request an early stop from another thread.
- `see_from_fen` provides static exchange analysis for a single move.

## Move Representation

Header: `include/prophet/move.h`

Moves are encoded as a `move_t` (64-bit). Helpers let you build and inspect moves.

```c
move_t mv = to_move(PAWN, E2, E4);
char *text = move_to_str(mv);
/* use text */
free(text);
```

`move_to_str` returns heap memory; callers must `free()` it.

## Move Generation

Header: `include/prophet/movegen.h`

Generate pseudo-legal moves from a FEN string:

```c
#include <prophet/movegen.h>

move_t moves[MAX_MOVES_PER_PLY];
int n = 0;
generate_moves_from_fen(moves, &n, fen, true, true);
```

## Evaluation

Header: `include/prophet/eval.h`

```c
#include <prophet/eval.h>

int32_t score = eval_from_fen(fen, false);
```

## Neural Network Evaluation (NNUE)

Header: `include/prophet/nn.h`

```c
#include <prophet/nn.h>

int rc = load_neural_network("nn-32-q.txt");
if (rc == 0) {
    int32_t score = nn_eval_from_fen(fen);
}
```

## Hash Tables

Header: `include/prophet/hash.h`

The hash-table API uses sizes in bytes (not MB).

```c
#include <prophet/hash.h>

resize_main_hash_table(256ULL * 1024 * 1024);
resize_pawn_hash_table(32ULL * 1024 * 1024);
clear_hash_tables();
```

You can also probe or store values using FEN strings.

## Error Codes

Public error codes are defined in `include/prophet/error_codes.h`.
APIs that return non-zero use these codes (or return values from underlying subsystems).
