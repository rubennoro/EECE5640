Overview:
There are two programs in this folder, both of which use CUDA for the common stencil computation. 
q2tile.cu utilizes tiling to improve the efficiency of the process, taking advantage of spatial locality in the memory of the 3D arrays.
q2reg.cu employs a regular implementaiton of the computation.
Both programs are initialized with 8 threads each. Their runtimes are calculated for 1000 iterations of kernel execution to 
get clear, consitent times to compare. 

To change n:
N is declared as a macro to define the 3D dimensions. Its value can be modified in both programs next to the headers.

To run:
1. Acquire a GPU node on the cluster
2. module load cuda/12.3.0

nvcc q2tile.cu -o q2tile
./q2tile

nvcc q2reg.cu -o q2reg
./q2reg

To view outputs of the files:
1. Modify line 97 of q2reg.cu to see the proper values of the indices in A.
2. Modify line 126 of q2tile.cu to see the proper values of the indices in A. 


