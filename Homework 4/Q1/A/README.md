This is part A of the problem 1, which includes a bash script and a C++ program for summing values between nodes with OpenMPI.

An integer variable is passed to every process, where successive processes add 1 to the value repeatedly. This is done for 64 processes, split amongst 4 nodes with 16 processes per node.

To Run:
sbatch p1_job.sh

To check results:
vim output_comm.txt

Example Output (up to Process 63):
Process 0 on node c0441 w/ val 1
Process 1 on node c0441 w/ val 2
Process 2 on node c0441 w/ val 3
Process 3 on node c0441 w/ val 4
Process 4 on node c0441 w/ val 5
Process 5 on node c0441 w/ val 6
Process 6 on node c0441 w/ val 7
Process 7 on node c0441 w/ val 8
Process 8 on node c0441 w/ val 9
Process 9 on node c0441 w/ val 10