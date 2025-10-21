
#!/bin/bash
#SBATCH --verbose
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=00:30:00
#SBATCH --job-name=RubensJob
#SBATCH --mem=100G
#SBATCH --partition=courses

module load OpenMPI/4.1.6

mpic++ p2b.cpp -o p2b

$SRUN mpirun -mca btl_bas:wq:quUie_warn_component_unused 0 p2b