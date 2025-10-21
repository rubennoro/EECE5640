#!/bin/bash
#SBATCH --verbose
#SBATCH --job-name=MPIComm
#SBATCH --partition=courses
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=00:50:00
#SBATCH --output=output1B.txt

module load OpenMPI/4.1.6

mpic++ p1b.cpp -o p1b

mpirun -np 64 --map-by ppr:16:node p1b