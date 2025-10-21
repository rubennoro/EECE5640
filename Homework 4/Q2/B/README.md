This program follows the part A flow:
It uses OpenMPI to compile randomly generated data into histogram bins. The head process initially generates 8,000,000 data points from 1 to 100,000 at random. It adds them into a vector, and shares the data in different chunks to all other processes. The processes then generate their own local histograms, which is reduced on the head process to form a main histogram.

There is no text output, instead a .out file is generated and can be opened to check output. An example of the output is in slurm.out, and the timing is also included. The number of bins was set to 32 for this code.

To experiment with different timing across 2 and 4 nodes, as well as processes per node, you can change these two in the batch script:
#SBATCH --nodes=2
#SBATCH --ntasks-per-node=4

To run:
sbatch p2b_job.sh

To see output:
1. ls (to see the files)
2. vim -insert file name- (to open the slurm file generated)

Example output:
Bin 0: 62439 values
Bin 1: 62403 values
Bin 2: 62366 values
Bin 3: 61993 values
Bin 4: 62235 values
Bin 5: 62666 values
Bin 6: 62531 values
Bin 7: 62419 values
Bin 8: 62549 values
Bin 9: 62753 values
Bin 10: 62621 values
Bin 11: 61992 values