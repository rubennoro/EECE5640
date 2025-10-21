This folder includes a C++ and CUDA implementation of a histogramming problem. It compares the runtimes of OpenMP in C++ versus CUDA on the GPU across different dataset sizes of 2^12 to 2^24 from values 1 to 100,000. The number of bins was set to 256 in both cases. 

To modify dataset sizes, modify the N macro at the top of each file.

To run:
g++ -fopenmp histo.cpp -o histo
./histo

For GPU:
1. Acquire a GPU node on the cluster
2. module load cuda/12.3.0
nvcc histo.cu -o cuhisto
./cuhisto


Time will print in all cases, but to show the class output:
In C++ program:
- Uncomment line 79 and recompile
In CUDA:
- Uncomment line 85 and recompile

