#!/bin/bash
#SBATCH --verbose
#SBATCH --job-name=MPIComm
#SBATCH --partition=courses
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=00:50:00
#SBATCH --output=output_comm.txt

module load OpenMPI/4.1.6

mpic++ p1.cpp -o p1

mpirun -np 64 --map-by ppr:16:node p1