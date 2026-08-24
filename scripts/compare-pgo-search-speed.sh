#!/usr/bin/env bash
#
# Compares real search throughput (not just perft/movegen) between a PGO
# build and a plain, non-PGO Release build. compare-pgo-perft.sh only
# exercises move generation, which is a poor proxy for real-game speed --
# most per-node cost in an actual search comes from eval/NN, which perft
# never touches. This runs a fixed-time search ('st') on each binary across
# a few representative positions and reports the reported node rate
# (kn/s) and total nodes searched for each, so a genuine PGO speed
# regression (e.g. from an under-representative training profile) shows up
# directly instead of being masked by perft-only numbers.
#
# Usage: scripts/compare-pgo-search-speed.sh [pgo-build-dir] [baseline-build-dir]
#   pgo-build-dir:      default build-pgo (must already be built, e.g. via
#                        scripts/build-pgo.sh)
#   baseline-build-dir: default build-baseline (configured+built by this
#                        script if not already present)
#
# Env vars:
#   ST_SEC: fixed search time in seconds per position (default 10)
#   NN:     neural network weights file (default test/resources/nn.txt)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

PGO_BUILD_DIR="${1:-$SRC_DIR/build-pgo}"
BASE_BUILD_DIR="${2:-$SRC_DIR/build-baseline}"
ST_SEC="${ST_SEC:-10}"
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
echo "Fixed search time per position: ${ST_SEC}s"

# same representative positions build-pgo.sh trains on: opening, tactical,
# quiet middlegame, endgame.
FENS=(
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R2Q1RK1 w - - 0 10"
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
)

run_search() {
    local exe="$1" fen="$2"
    {
        echo "new"
        echo "setboard $fen"
        echo "st $ST_SEC"
        echo "go"
        echo "ping 1"
        echo "quit"
    } | "$exe" -n "$NN" 2>&1 | grep -E '^# (search time|nodes):'
}

parse_rate() { sed -nE 's/.*rate: ([0-9]+) kn\/s.*/\1/p'; }
parse_nodes() { sed -nE 's/^# nodes: ([0-9]+)k.*/\1/p'; }

total_pgo_rate=0
total_base_rate=0
count=0

for fen in "${FENS[@]}"; do
    echo
    echo "=== position: $fen ==="

    pgo_out="$(run_search "$PGO_EXE" "$fen")"
    base_out="$(run_search "$BASE_EXE" "$fen")"

    pgo_rate="$(echo "$pgo_out" | parse_rate)"
    base_rate="$(echo "$base_out" | parse_rate)"
    pgo_nodes="$(echo "$pgo_out" | parse_nodes)"
    base_nodes="$(echo "$base_out" | parse_nodes)"

    echo "  PGO:      ${pgo_nodes}k nodes, ${pgo_rate} kn/s"
    echo "  Baseline: ${base_nodes}k nodes, ${base_rate} kn/s"

    if [[ -n "$pgo_rate" && -n "$base_rate" && "$base_rate" -gt 0 ]]; then
        awk -v p="$pgo_rate" -v b="$base_rate" 'BEGIN { printf "  PGO/baseline speed ratio: %.2fx\n", p / b }'
        total_pgo_rate=$((total_pgo_rate + pgo_rate))
        total_base_rate=$((total_base_rate + base_rate))
        count=$((count + 1))
    else
        echo "  (could not parse rate for one or both binaries)"
    fi
done

echo
echo "=== summary (mean of per-position rates, ${count} positions) ==="
if [[ "$count" -gt 0 ]]; then
    awk -v p="$total_pgo_rate" -v b="$total_base_rate" -v n="$count" \
        'BEGIN { printf "  PGO:      %.0f kn/s avg\n  Baseline: %.0f kn/s avg\n  Ratio:    %.2fx\n", p/n, b/n, p/b }'
else
    echo "  no usable data"
fi
