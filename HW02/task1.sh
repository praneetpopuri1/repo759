#!/usr/bin/env bash
#SBATCH --partition=instruction
#SBATCH --job-name=task1
#SBATCH --output=task1.out
#SBATCH --error=task1.err
#SBATCH --time=0-00:10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=10G

# Scaling analysis for task 1: one line per run, formatted as
#   n time_ms first_element last_element
# for n = 2^10, ..., 2^30. Plot it with: python plot_task1.py task1.out
# (n = 2^30 needs two 4 GiB float arrays, hence --mem=10G.)

g++ scan.cpp task1.cpp -Wall -O3 -std=c++17 -o task1

for p in {10..30}; do
    n=$((2 ** p))
    echo "$n $(./task1 "$n" | paste -sd ' ' -)"
done
