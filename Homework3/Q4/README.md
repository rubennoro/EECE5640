The matmulblas.c program was used to compare the runtime of traditional matrix multiplication
with OpenBLAS optimization. OpenBLAS is a high performance linear algebra efficiency library 
that enables tremendous speedup of tasks such as matmul.

Important Variables:
The N variable is used to change the size of the matrices to measure different runtimes.

To run:
1. Acquire a cluster node, and then a second one
First:
srun --partition=courses --constraint=cascadelake --nodes=1 --ntasks-per-node=1 --cpus-per-task=4 --time=00:10:00 --pty bash

Second:
srun --partition=courses --constraint=cascadelake --nodes=1 --ntasks-per-node=1 --cpus-per-task=32 --time=00:10:00 --pty bash

2. Load OpenBLAS
module load OpenBLAS/0.3.29

3. Compile Code
gcc matmulblas.c -o matmulblas -lopenblas

4. Run Code
./matmulblas

Example Output:
OpenBlas Val: 1024.00
OpenBlas Time: 3.59

The output will print the expected matrix multiplication value and the speed in milliseconds.