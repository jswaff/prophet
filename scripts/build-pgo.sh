#!/usr/bin/env bash
#
# Two-pass Profile-Guided Optimization (PGO) build for prophet.
#
# Pass 1: configure + build an instrumented binary (-DPROFILE_GUIDED=GENERATE),
#         then run it against a training workload (perft + a few searches on
#         representative positions) to collect profile data.
# Pass 2: reconfigure + rebuild an optimized binary (-DPROFILE_GUIDED=USE)
#         from that profile data.
#
# GCC and Clang only. For MSVC, see the "Windows / MSVC PGO" section in
# README.md for the manual steps.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

BUILD_DIR="build-pgo"
PERFT_DEPTH=5
SD=8
TIMEOUT_SEC=30
NN="$SRC_DIR/test/resources/nn.txt"
JOBS="$(command -v nproc >/dev/null && nproc || echo 4)"
CLEAN=0
FENS=()
CMAKE_ARGS=()

DEFAULT_FENS=(
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP2PPP/R2Q1RK1 w - - 0 10"
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"
)

usage() {
    cat <<EOF
Usage: $(basename "$0") [options]

Options:
  --build-dir DIR     Build directory to use (default: $BUILD_DIR)
  --source-dir DIR    Path to repo root (default: auto-detected: $SRC_DIR)
  --perft-depth N     Perft depth for the training run (default: $PERFT_DEPTH)
  --sd N              Search depth ('sd') per training position (default: $SD)
  --timeout N         Time-control backstop in seconds ('st'), sent before
                      every 'go' alongside 'sd' (default: $TIMEOUT_SEC).
                      'sd' alone only bounds the number of iterations, not
                      the wall-clock cost of reaching one -- without a time
                      control, a slow/tactical position can run 'go'
                      indefinitely and hang the subsequent 'ping'.
  --fen FEN           Representative FEN to train on; repeatable.
                      (default: four built-in FENs - opening, tactical,
                      quiet middlegame, endgame)
  --nn PATH           Neural network weights file (default: $NN)
  -j, --jobs N        Parallel build jobs (default: $JOBS)
  --clean             Wipe --build-dir before starting
  --cmake-arg ARG     Extra argument passed through to both cmake configure
                      invocations; repeatable (e.g. --cmake-arg=-DWITH_AVX=OFF)
  -h, --help          Show this help and exit
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --build-dir) BUILD_DIR="$2"; shift 2 ;;
        --source-dir) SRC_DIR="$2"; shift 2 ;;
        --perft-depth) PERFT_DEPTH="$2"; shift 2 ;;
        --sd) SD="$2"; shift 2 ;;
        --timeout) TIMEOUT_SEC="$2"; shift 2 ;;
        --fen) FENS+=("$2"); shift 2 ;;
        --nn) NN="$2"; shift 2 ;;
        -j|--jobs) JOBS="$2"; shift 2 ;;
        --clean) CLEAN=1; shift ;;
        --cmake-arg) CMAKE_ARGS+=("$2"); shift 2 ;;
        -h|--help) usage; exit 0 ;;
        *) echo "error: unknown option '$1'" >&2; usage >&2; exit 1 ;;
    esac
done

if [[ ${#FENS[@]} -eq 0 ]]; then
    FENS=("${DEFAULT_FENS[@]}")
fi

command -v cmake >/dev/null || { echo "error: cmake not found" >&2; exit 1; }

if [[ "$BUILD_DIR" != /* ]]; then
    BUILD_DIR="$SRC_DIR/$BUILD_DIR"
fi

if [[ "$CLEAN" == "1" ]]; then
    rm -rf "$BUILD_DIR"
fi
mkdir -p "$BUILD_DIR/pgo-data"

echo "==> Pass 1/2: configuring + building instrumented (GENERATE) binary"
cmake -S "$SRC_DIR" -B "$BUILD_DIR" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPROFILE_GUIDED=GENERATE \
    -DPROFILE_GUIDED_DATA_DIR="$BUILD_DIR/pgo-data" \
    -DBUILD_TEST_EXECUTABLE=OFF \
    "${CMAKE_ARGS[@]}"
cmake --build "$BUILD_DIR" --config Release -j"$JOBS"

EXE_PATH=""
for candidate in "$BUILD_DIR"/prophet "$BUILD_DIR"/prophet-avx2; do
    if [[ -x "$candidate" ]]; then
        EXE_PATH="$candidate"
        break
    fi
done
[[ -n "$EXE_PATH" ]] || { echo "error: could not locate built prophet executable in $BUILD_DIR" >&2; exit 1; }

echo "==> Running training workload against $EXE_PATH"
{
    echo "perft $PERFT_DEPTH"
    for fen in "${FENS[@]}"; do
        echo "new"
        echo "setboard $fen"
        echo "st $TIMEOUT_SEC"
        echo "sd $SD"
        echo "go"
        echo "ping 1"
    done
    echo "quit"
} | LLVM_PROFILE_FILE="$BUILD_DIR/pgo-data/prophet-train.profraw" "$EXE_PATH" -n "$NN" >/dev/null

shopt -s nullglob
PROFILE_ARTIFACTS=("$BUILD_DIR"/pgo-data/*.gcda "$BUILD_DIR"/pgo-data/*.profraw)
shopt -u nullglob
[[ ${#PROFILE_ARTIFACTS[@]} -gt 0 ]] || { echo "error: no profile data (.gcda/.profraw) found after training run" >&2; exit 1; }

COMPILER_ID=$(cat "$BUILD_DIR/compiler_id.txt")

if [[ "$COMPILER_ID" == "Clang" || "$COMPILER_ID" == "AppleClang" ]]; then
    echo "==> Merging Clang .profraw -> default.profdata"
    command -v llvm-profdata >/dev/null || { echo "error: llvm-profdata not found in PATH (install LLVM tools)" >&2; exit 1; }
    llvm-profdata merge -output="$BUILD_DIR/pgo-data/default.profdata" "$BUILD_DIR"/pgo-data/*.profraw
elif [[ "$COMPILER_ID" == "MSVC" ]]; then
    echo "error: MSVC detected; this script does not automate the Windows/MSVC PGO flow." >&2
    echo "       See README.md 'Windows / MSVC PGO' section for manual steps." >&2
    exit 1
fi
# GNU: .gcda files are already directly usable, nothing to merge

echo "==> Pass 2/2: reconfiguring + rebuilding optimized (USE) binary"
cmake -S "$SRC_DIR" -B "$BUILD_DIR" \
    -DPROFILE_GUIDED=USE \
    "${CMAKE_ARGS[@]}"
cmake --build "$BUILD_DIR" --config Release --clean-first -j"$JOBS"

echo "==> Done. Optimized binary: $EXE_PATH"
