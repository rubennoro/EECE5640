The matmulAVX.c program used the AVX512 instructions in CPUs on the Explorer Cluster 
to improve the efficiency of matrix multiplication. It stores and SIMD operates with 16 single-precision floats, for a total of 512 bits.

Important Variables: 
The N variable was used to modify the matrix sizes to test speeds of matrix multiplication 
with AVX.

To run:
1. First Get Node with Constraint Cascade Lake
srun --partition=courses --constraint=cascadelake --nodes=1 --ntasks-per-node=1 --cpus-per-task=4 --time=00:10:00 --pty bash
2. Compile code
gcc matmulAVX.c -o matmulAVX -mavx512f -S
3. Run code
./matmulAVX

To inspect assembly:
vim matmulAVX

Example Output (At N=512):
No AVX Time : 428.106915
No AVX Result: 1024.000000
AVX Time: 43.429951
AVX Result: 1024.000000