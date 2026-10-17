#!/usr/bin/env bash
# Local (macOS) stand-in for the Euler Slurm jobs: builds the three tasks with
# Homebrew's libomp and writes the timing data the plots are made from.
# Usage: ./run_local.sh
set -euo pipefail
cd "$(dirname "$0")"

LIBOMP="$(brew --prefix libomp)"
OMP_FLAGS=(-Xpreprocessor -fopenmp "-I$LIBOMP/include" "-L$LIBOMP/lib" -lomp)

g++ task1.cpp matmul.cpp -Wall -O3 -std=c++17 -o task1 "${OMP_FLAGS[@]}"
g++ task2.cpp convolution.cpp -Wall -O3 -std=c++17 -o task2 "${OMP_FLAGS[@]}"
g++ task3.cpp msort.cpp -Wall -O3 -std=c++17 -o task3 "${OMP_FLAGS[@]}"

# Each data line is "x time_ms". A laptop is much noisier than a dedicated
# Euler node, so each point is the median of RUNS runs; the time is the third
# line of program output.
RUNS=5
median_time() {
    for _ in $(seq 1 "$RUNS"); do "$@" | sed -n 3p; done |
        sort -g | sed -n "$(((RUNS + 1) / 2))p"
}

: > task1.out
for t in $(seq 1 20); do
    echo "$t $(median_time ./task1 1024 "$t")" >> task1.out
done

: > task2.out
for t in $(seq 1 20); do
    echo "$t $(median_time ./task2 1024 "$t")" >> task2.out
done

: > task3_ts.out
for p in $(seq 1 10); do
    ts=$((2 ** p))
    echo "$ts $(median_time ./task3 1000000 8 "$ts")" >> task3_ts.out
done

best_ts=$(sort -k2 -g task3_ts.out | head -1 | cut -d' ' -f1)
: > task3_t.out
for t in $(seq 1 20); do
    echo "$t $(median_time ./task3 1000000 "$t" "$best_ts")" >> task3_t.out
done
echo "best ts = $best_ts"
