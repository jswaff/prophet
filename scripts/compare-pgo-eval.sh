#!/usr/bin/env bash
#
# Checks whether a PGO build and a plain, non-PGO Release build actually
# agree on evaluation and search decisions -- not just speed. Node
# throughput being roughly equal (see compare-pgo-search-speed.sh) doesn't
# rule out PGO/LTO changing floating-point codegen in the AVX-vectorized NN
# inference path (instruction selection, FMA contraction, summation order),
# which can silently shift eval scores and therefore move choices without
# any crash or warning.
#
# For each test position this compares, between the two binaries:
#   1. 'eval' output (handcrafted score and NN score) -- isolates the
#      leaf-level eval/NN code from search entirely.
#   2. the move and score chosen by a fixed-depth search -- catches
#      anything beyond eval (move ordering, hash interactions, pruning)
#      that diverges between the builds.
# Any mismatch is real: search and eval are deterministic here (no
# randomization, single-threaded decision path), so identical inputs must
# produce identical outputs regardless of which binary produced them.
#
# Usage: scripts/compare-pgo-eval.sh [pgo-build-dir] [baseline-build-dir]
#   pgo-build-dir:      default build-pgo (must already be built, e.g. via
#                        scripts/build-pgo.sh)
#   baseline-build-dir: default build-baseline (configured+built by this
#                        script if not already present)
#
# Env vars:
#   SD: fixed search depth per position (default 10)
#   NN: neural network weights file (default test/resources/nn.txt)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

PGO_BUILD_DIR="${1:-$SRC_DIR/build-pgo}"
BASE_BUILD_DIR="${2:-$SRC_DIR/build-baseline}"
SD="${SD:-10}"
NN="${NN:-$SRC_DIR/test/resources/nn.txt}"
JOBS="$(command -v nproc >/dev/null && nproc || echo 4)"

if [[ "$PGO_BUILD_DIR" != /* ]]; then
    PGO_BUILD_DIR="$SRC_DIR/$PGO_BUILD_DIR"
fi
if [[ "$BASE_BUILD_DIR" != /* ]]; then
    BASE_BUILD_DIR="$SRC_DIR/$BASE_BUILD_DIR"
fi

find_exe() {
    local dir="$1"
    for candidate in "$dir"/prophet "$dir"/prophet-avx2; do
        if [[ -x "$candidate" ]]; then
            echo "$candidate"
            return 0
        fi
    done
    return 1
}

find_exe "$PGO_BUILD_DIR" >/dev/null || { echo "error: no built executable found in $PGO_BUILD_DIR (run scripts/build-pgo.sh first)" >&2; exit 1; }

echo "==> Configuring + building plain (non-PGO) baseline Release build in $BASE_BUILD_DIR"
cmake -S "$SRC_DIR" -B "$BASE_BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DBUILD_TEST_EXECUTABLE=OFF
cmake --build "$BASE_BUILD_DIR" --config Release -j"$JOBS"

PGO_EXE="$(find_exe "$PGO_BUILD_DIR")"
BASE_EXE="$(find_exe "$BASE_BUILD_DIR")"

echo "PGO binary:      $PGO_EXE"
echo "Baseline binary: $BASE_EXE"
echo "Fixed search depth per position: $SD"

# a diverse battery: opening/tactical/middlegame/endgame positions from
# build-pgo.sh's default set, plus a few more spanning both colors to move,
# including a couple of color-mirrored pairs (each position and its
# flipped-color equivalent) since the observed match regression was much
# worse playing Black.
FENS=(
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq - 0 1"
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R2Q1RK1 w - - 0 10"
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R2Q1RK1 b - - 0 10"
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 b - - 0 1"
    "rnbq1rk1/ppp1bppp/4pn2/3p4/2PP4/2N1PN2/PP3PPP/R1BQKB1R w KQ - 0 7"
    "2rq1rk1/pp1bppbp/2np1np1/8/3NP3/2N1BP2/PPPQ2PP/2KR1B1R b - - 0 10"
    "6k1/5ppp/8/8/8/8/5PPP/6K1 w - - 0 1"
    "4r1k1/5ppp/8/8/8/8/5PPP/4R1K1 b - - 0 1"
)

diff_eval() {
    local fen="$1"
    local pgo_out base_out
    pgo_out="$({ echo "new"; echo "setboard $fen"; echo "eval"; echo "quit"; } | "$PGO_EXE" -n "$NN" 2>&1 | grep -E '^(eval|nn) score:')"
    base_out="$({ echo "new"; echo "setboard $fen"; echo "eval"; echo "quit"; } | "$BASE_EXE" -n "$NN" 2>&1 | grep -E '^(eval|nn) score:')"

    if [[ "$pgo_out" == "$base_out" ]]; then
        echo "  eval: MATCH ($(echo "$pgo_out" | tr '\n' ' '))"
    else
        echo "  eval: MISMATCH"
        echo "    PGO:      $(echo "$pgo_out" | tr '\n' ' ')"
        echo "    Baseline: $(echo "$base_out" | tr '\n' ' ')"
        return 1
    fi
}

diff_search() {
    local fen="$1"
    local pgo_raw base_raw pgo_move base_move pgo_score base_score
    # 'st' is required here, not just 'sd': with no time control set at all,
    # max_time_ms stays 0 and iterate.c's "stop if elapsed > max_time_ms/2"
    # check (elapsed > 0) trips on the first iteration boundary the ms clock
    # happens to tick over on -- a wall-clock race, not a depth bound. 'st'
    # (with an ample backstop) is what makes 'sd' actually deterministic.
    pgo_raw="$({ echo "new"; echo "setboard $fen"; echo "st 300"; echo "sd $SD"; echo "go"; echo "ping 1"; echo "quit"; } | "$PGO_EXE" -n "$NN" 2>&1)"
    base_raw="$({ echo "new"; echo "setboard $fen"; echo "st 300"; echo "sd $SD"; echo "go"; echo "ping 1"; echo "quit"; } | "$BASE_EXE" -n "$NN" 2>&1)"

    pgo_move="$(echo "$pgo_raw" | grep -E '^move ' | tail -1)"
    base_move="$(echo "$base_raw" | grep -E '^move ' | tail -1)"
    pgo_score="$(echo "$pgo_raw" | grep -E '^# depth:' | tail -1)"
    base_score="$(echo "$base_raw" | grep -E '^# depth:' | tail -1)"

    if [[ "$pgo_move" == "$base_move" && "$pgo_score" == "$base_score" ]]; then
        echo "  search (sd $SD): MATCH ($pgo_move, $pgo_score)"
    else
        echo "  search (sd $SD): MISMATCH"
        echo "    PGO:      $pgo_move, $pgo_score"
        echo "    Baseline: $base_move, $base_score"
        return 1
    fi
}

mismatches=0
for fen in "${FENS[@]}"; do
    echo
    echo "=== position: $fen ==="
    diff_eval "$fen" || mismatches=$((mismatches + 1))
    diff_search "$fen" || mismatches=$((mismatches + 1))
done

echo
if [[ "$mismatches" -eq 0 ]]; then
    echo "=== all positions matched between PGO and baseline ==="
else
    echo "=== $mismatches mismatch(es) found -- see above ==="
fi
exit $((mismatches > 0 ? 1 : 0))
