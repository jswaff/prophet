#!/usr/bin/env bash
#
# Verifies that a PGO-optimized build produces identical perft node counts
# to a plain, non-PGO Release build, and reports the timing difference
# between them. Movegen is deterministic, so node counts must match exactly
# -- PGO should only change speed, never search/movegen correctness (node
# counts are compared on their own, separately from timing, since timing
# naturally varies run to run and is not a correctness signal). Builds a
# plain baseline Release build if one doesn't already exist, then runs perft
# on both binaries across a few representative positions (including
# Kiwipete, a classic movegen stress-test position), diffs the node counts,
# and prints each build's time/rate at the deepest ply plus the speedup.
#
# Usage: scripts/compare-pgo-perft.sh [pgo-build-dir] [baseline-build-dir]
#   pgo-build-dir:      default build-pgo (must already be built, e.g. via
#                        scripts/build-pgo.sh)
#   baseline-build-dir: default build-baseline (configured+built by this
#                        script if not already present)
#
# Env vars:
#   PERFT_DEPTH: perft depth to run (default 6)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

PGO_BUILD_DIR="${1:-$SRC_DIR/build-pgo}"
BASE_BUILD_DIR="${2:-$SRC_DIR/build-baseline}"
PERFT_DEPTH="${PERFT_DEPTH:-6}"
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

# representative positions: empty string means "use default starting position"
# (no 'setboard' sent); the second is Kiwipete, a classic perft stress-test
# position with castling/en-passant/promotions all in play.
FENS=(
    ""
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
)

run_perft() {
    local exe="$1"
    local fen="$2"
    {
        if [[ -n "$fen" ]]; then
            echo "setboard $fen"
        fi
        echo "perft $PERFT_DEPTH"
        echo "quit"
    } | "$exe" | grep '^depth '
}

nodes_only() {
    sed -E 's/^(depth [0-9]+) - nodes: ([0-9]+).*/\1 nodes:\2/'
}

status=0
for fen in "${FENS[@]}"; do
    label="${fen:-startpos}"
    echo
    echo "=== position: $label (depth $PERFT_DEPTH) ==="

    pgo_raw="$(run_perft "$PGO_EXE" "$fen")"
    base_raw="$(run_perft "$BASE_EXE" "$fen")"
    pgo_nodes="$(echo "$pgo_raw" | nodes_only)"
    base_nodes="$(echo "$base_raw" | nodes_only)"

    if [[ "$pgo_nodes" == "$base_nodes" ]]; then
        echo "MATCH"
        echo "$pgo_nodes" | tail -1
    else
        echo "MISMATCH"
        echo "--- PGO ---"
        echo "$pgo_nodes"
        echo "--- baseline ---"
        echo "$base_nodes"
        status=1
    fi

    # timing comes from the final (deepest) perft line of each run
    pgo_last="$(echo "$pgo_raw" | tail -1)"
    base_last="$(echo "$base_raw" | tail -1)"
    pgo_ms="$(echo "$pgo_last" | sed -E 's/.*time: ([0-9]+)ms.*/\1/')"
    base_ms="$(echo "$base_last" | sed -E 's/.*time: ([0-9]+)ms.*/\1/')"
    pgo_rate="$(echo "$pgo_last" | sed -E 's/.*rate: ([0-9]+) kn\/s.*/\1/')"
    base_rate="$(echo "$base_last" | sed -E 's/.*rate: ([0-9]+) kn\/s.*/\1/')"

    echo "Timing (depth $PERFT_DEPTH):"
    echo "  PGO:      ${pgo_ms}ms (${pgo_rate} kn/s)"
    echo "  Baseline: ${base_ms}ms (${base_rate} kn/s)"
    if [[ "$pgo_ms" -gt 0 && "$base_ms" -gt 0 ]]; then
        awk -v p="$pgo_ms" -v b="$base_ms" 'BEGIN { printf "  Speedup:  %.2fx\n", b / p }'
    else
        echo "  Speedup:  (too fast at this depth to measure reliably; increase PERFT_DEPTH)"
    fi
done

exit $status
