The matmul.c program optimizes matrix multiplication for two sparse matrices as well as two dense matrices. The optimizations are specified in greater detail in the Q2 document. These optimizations utilize common HPC libraries including AVX512 and OpenMP. The runtime of one of either optimized or regular matrix multiplications for both sparse and dense are run, and their outputs and speeds are printed. 

Important Notes:
The code is setup to run either the optimized or regular versions of dense and sparse matmul. 
It will not run both versions at the same time. There is a marking in the code between the 
start and finish clock variables for dense and sparse where you can comment out or add in the 
function you'd like to test.

Example:
  start = CLOCK();
  
  //Choose to either run optimized version or regular version
  //regularSparse(a, b, c);
  optimSparse(a, b, c);

  finish = CLOCK();


To run:
1. Get the AVX CPU node
srun --partition=courses --constraint=cascadelake --nodes=1 --ntasks-per-node=1 --cpus-per-task=4 --time=00:10:00 --pty bash
2. Compile
gcc -O3 -mavx512f -mfma -fopenmp -o matmul matmul.c
3. Run
./matmul


Example Output (For optimized dense and sparse):

starting dense matrix multiply 
a result 4.44488e+08 
The total time for matrix multiplication with dense matrices = 76.403660 ms
starting sparse matrix multiply 
A result 0 
The total time for matrix multiplication with sparse matrices = 4.387691 ms
The sparsity of the a and b matrices = 0.750977

NOTE:
Though the A result says 0 above, this is because it is part of the sparse matrix portion
and will therefore contain the value it is initially set to, which is 0.