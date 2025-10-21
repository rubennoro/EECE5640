#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cuda_runtime.h>

// Setting n and the tile size for tiling
#define n 512

double CLOCK(){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec * 1000) + (t.tv_nsec*1e-6);
}

// Kernel execution
__global__ void nn(float (*a)[n][n], float (*b)[n][n]){

    // Global indices
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;
    int k = blockIdx.z * blockDim.z + threadIdx.z;

    // Only compute interior points (1 ≤ i,j,k ≤ n-2)
    if (i > 0 && i < n-1 && j > 0 && j < n-1 && k > 0 && k < n-1) {
        a[i][j][k] = 0.75f * (
            b[i-1][j][k] + b[i+1][j][k] +  // x neighbors
            b[i][j-1][k] + b[i][j+1][k] +  // y neighbors
            b[i][j][k-1] + b[i][j][k+1]    // z neighbors
        );
    }

    // Ensure all threads have reached this point before continuation
    __syncthreads();
    

}

int main(int argc, char* argv[]){

    double start, end, total;

    //dim3 dimensions = dim3(n, n, n);
    float a[n][n][n], b[n][n][n];
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            for(int k = 0; k < n; k++){
                b[i][j][k] = 2.0f;
                a[i][j][k] = 0.0f;
            }
        }
    }

    // Allocate device memory for 3D arrays
    float (*cuA)[n][n], (*cuB)[n][n];
    cudaMalloc(&cuA, sizeof(float)*n*n*n);
    cudaMalloc(&cuB, sizeof(float)*n*n*n);
    
    // Copy data to device
    cudaMemcpy(cuB, b, sizeof(float)*n*n*n, cudaMemcpyHostToDevice);
    cudaMemcpy(cuA, a, sizeof(float)*n*n*n, cudaMemcpyHostToDevice);

    // Dimensions
    /*
    For 32 x 32 x 32:
    - 6 x 6 x 6 threads per block
    - 8 x 8 x 8 blocks in grid
    */
	int threads = 8;
    dim3 gridDim((n + threads - 1) / threads, (n + threads - 1) / threads, (n + threads - 1) / threads);
    dim3 blockDim(threads, threads, threads);

    // track start of operation
    start = CLOCK();

    // Execute kernel several times
    for(int i = 0; i < 1000; i++){
        nn<<<gridDim, blockDim>>>(cuA, cuB);
    }
    
    cudaDeviceSynchronize();
    // track end of operation
    end = CLOCK();

	//cudaDeviceSynchronize();

    // copy memory back to device 3d array
    cudaMemcpy(a, cuA, sizeof(float) * n * n * n, cudaMemcpyDeviceToHost);

    // Time calculations and printing
    total = end - start;
    std::cout << "Time: " << total << " ms" << std::endl;
    
    // Checking answers
    for(int i = 0; i < 10; i++){
        //std::cout << a[1][1][i] << std::endl;
    }

    cudaFree(cuA);
    cudaFree(cuB);

    return 0;
}