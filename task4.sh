#!/bin/bash
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --job-name=FirstSlurm
#SBATCH --output=FirstSlurm.out
#SBATCH --error=FirstSlurm.err

hostname
