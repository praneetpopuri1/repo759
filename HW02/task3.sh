#!/usr/bin/env bash
#SBATCH --partition=instruction
#SBATCH --job-name=task3
#SBATCH --output=task3.out
#SBATCH --error=task3.err
#SBATCH --time=0-00:10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1

g++ task3.cpp matmul.cpp -Wall -O3 -std=c++17 -o task3

./task3
