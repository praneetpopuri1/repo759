#!/usr/bin/env bash
#SBATCH --partition=instruction
#SBATCH --job-name=task2
#SBATCH --output=task2.out
#SBATCH --error=task2.err
#SBATCH --time=0-00:10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1

g++ convolution.cpp task2.cpp -Wall -O3 -std=c++17 -o task2

for args in "1024 3" "1024 7" "4096 3"; do
    echo "./task2 $args"
    ./task2 $args
done
