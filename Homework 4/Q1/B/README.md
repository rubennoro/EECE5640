This is part B of the problem 1, which includes a bash script and a C++ program for summing and subtracting values between nodes with OpenMPI.

An integer variable is passed to every process, where successive processes add 1 to the value repeatedly. This is done for 64 processes, split amongst 4 nodes with 16 processes per node. After the value of 64 is reached, processes decrement the value by 2 until it reaches 0.

To Run:
sbatch p1b_job.sh

To check results:
vim output1B.txt

Example Output (up to Process 63):
Process 0 on node c0441 w/ val 1
Process 1 on node c0441 w/ val 2
Process 2 on node c0441 w/ val 3

and (Down to 0):

Process 61 on node c0444w/ val 62
Process 62 on node c0444w/ val 63
Process 63 on node c0444w/ val 64
Process 0 on node c0441w/ val 62
Process 1 on node c0441w/ val 60
Process 2 on node c0441w/ val 58