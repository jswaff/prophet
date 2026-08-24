# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Prophet is an Xboard/Winboard-compatible chess engine written in C (library) with a C++ (gtest) test
suite. It has no GUI; it speaks the xboard protocol over stdin/stdout. It supports both a handcrafted
evaluation and an NNUE-style neural network evaluation (enabled via `-n <weights file>`).

## Build

Standard build:

```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

Useful CMake options (see top of `CMakeLists.txt`):
- `BUILD_TEST_EXECUTABLE` (default `ON`) - builds the `prophet_test` gtest binary.
- `WITH_AVX` (default `ON`) - builds with AVX2 intrinsics; the executable is named `prophet-avx2` when on.
- `DEBUG_AVX` (default `OFF`).
- `PROFILE_GUIDED` (`OFF`/`GENERATE`/`USE`) - see PGO section below.

### Profile-Guided Optimization (PGO)

On Linux/GCC or Clang, use the helper script rather than driving `PROFILE_GUIDED` by hand:

```
./scripts/build-pgo.sh            # instrument -> train -> merge -> optimized rebuild in build-pgo/
./scripts/build-pgo.sh --help     # custom FENs, --sd, --perft-depth, --tc-inc/--time-pressure-ms, --build-dir, etc.
./scripts/compare-pgo-perft.sh    # sanity-check perft output/timing of a PGO build vs. a plain build
```

Windows/MSVC has PowerShell equivalents (`scripts/build-pgo.ps1`, `scripts/compare-pgo-perft.ps1`) but they
are **unverified against a real MSVC toolchain** - see the "Windows / MSVC PGO" section in README.md for the
manual fallback steps and known gotchas (e.g. always pair `sd` with an `st <seconds>` backstop when training,
since `sd` alone doesn't bound wall-clock time).

Manual PGO invocation (GCC/Clang), if not using the script:
```
cmake -B build-pgo -DPROFILE_GUIDED=GENERATE && cmake --build build-pgo
# run a representative workload (perft + a few timed searches) to produce profile data
cmake -B build-pgo -DPROFILE_GUIDED=USE && cmake --build build-pgo --clean-first
```

## Test

Tests are gtest-based (`prophet_test`), registered with ctest via `gtest_discover_tests`.

```
cd build
ctest                                    # run all tests
ctest -R <regex>                         # run a subset of tests by name
./prophet_test --gtest_filter=Suite.Case # run a single test directly
```

Test resources (FEN files, NN weights, etc.) live under `test/resources/` and are pointed to at test time via
the `RESOURCE_DIR` environment variable / CMake cache var (see `CMakeLists.txt`, `test/util/test_paths.cpp`).

## Architecture

The project is a CMake library (`prophetlib`) plus a thin `prophet`/`prophet-avx2` executable
(`src/main.c`) that links it. Public API surface is intentionally small - only the headers under
`include/prophet/` are installed; everything under `src/` is private implementation, split by concern:

- `src/command/` - the xboard protocol command loop. `parse_command.c` holds a flat
  `function_table[]` mapping command name -> handler function (`cmd_func_t`); this is the place to look
  when adding or tracing a protocol command. Xboard-specific handlers live in `src/command/xboard/`;
  non-xboard commands (`db`, `eval`, `perft`) sit directly under `src/command/`. `main.c` ->
  `command_loop()` -> `parse_and_execute()` -> `parse_command()` dispatch is the request path for every
  line read from stdin.
- `src/search/` - iterative-deepening alpha-beta search (`iterate.c`, `search.c`), quiescence search
  (`qsearch.c`), move ordering (`init_move_ordering.c`, `next.c`, `mvvlva.c`), SEE (`see.c`), and search
  stopping/time-management (`stop.c`). `iterate_from_fen`/`iterate_from_move_history` in
  `include/prophet/search.h` are the public entry points.
- `src/eval/` - handcrafted, tapered (midgame/endgame-blended) positional evaluation, one file per
  piece type/term (`eval_pawn.c`, `eval_king_safety.c`, `outpost.c`, `trapped_bishop.c`, etc.), combined in
  `eval.c` / `eval_taper.c`.
- `src/nn/` - the NNUE-style neural network evaluation: `load_network.c` parses weight files,
  `neural_network.c` holds the forward-pass/inference code (including AVX intrinsics, largely ported from
  David Carteau's Cerebrum library per the README), `nnue_accumulators.c` maintains incremental
  accumulator state as moves are made/unmade, `nn_eval.c` is the public entry point. Network weights are
  trained externally by a separate project, [chess-trainer](https://github.com/jswaff/chess-trainer).
- `src/position/` - board representation, move application/undo, Zobrist hashing, draw detection
  (repetition, 50-move, insufficient material).
- `src/movegen/` - pseudo-legal move generation per piece type, plus `perft.c`.
- `src/hash/` - transposition table and pawn hash table (separately sized/configurable via `-h`/`-p`).
- `src/bitmap/` - bitboard primitives (popcount, LSB, rays, file/rank masks).
- `src/util/` - cross-platform shims (`posix_compat.c`, `getopt_compat.c`, `thread_compat.c`) that let
  the same C source build under both POSIX toolchains and MSVC, plus PRNG, string, timing, and output
  (`plog`) helpers.

`test/` mirrors `src/`'s directory layout one-to-one, one `test_<file>.cpp` per source file under test.

The engine is single-binary/single-process; concurrency (search-in-background-while-accepting-xboard-input)
is handled through `src/util/thread_compat.c` and the `stop_search`/`skip_time_checks` volatile flags
checked from within the search (see `src/command/xboard/block_on_search_thread.c`).

## Notes

- Compiler warnings are treated as errors (`-Wall -Werror -Wextra` on GCC/Clang, `/W4 /WX` on MSVC) -
  expect a clean build to fail on any new warning.
- `docs/API.md` documents the public C API (init, search, move generation, eval, NNUE, hash tables) for
  consumers linking against `prophetlib` (e.g. the sibling Java engine/GUI project, chess4j).
- Versioning and release history live in `README.md`; bump the version string in `src/main.c`'s welcome
  message and `CMakeLists.txt`'s `project()` call together when cutting a release.
