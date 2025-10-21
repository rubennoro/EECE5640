#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <cuda_runtime.h>

// Setting n and the tile size for tiling
#define n 512
#define tileSize 8

double CLOCK(){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec * 1000) + (t.tv_nsec*1e-6);
}

__global__ void nn(float (*a)[n][n], float (*b)[n][n]){

    // Shared memory of halo array, covering surrounding indices of each thread
    __shared__ float sharedB[tileSize + 2][tileSize + 2][tileSize + 2];

    // Thread Indices
    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int tz = threadIdx.z;

    // Block Indices
    int bx = 1 + blockIdx.x * tileSize;
    int by = 1 + blockIdx.y * tileSize;
    int bz = 1 + blockIdx.z * tileSize;

    // Calcluating the indices themselves
    int x = bx + tx - 1;
    int y = by + ty - 1;
    int z = bz + tz - 1;

    //Adding values to shared memory halo
    if (x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n) {
        // Load data (including halo)
        sharedB[tx][ty][tz] = b[x][y][z];
    }

    // Ensure all threads have reached this point before continuation
    __syncthreads();

    // The edges will NOT be modified
    if (tx > 0 && tx <= tileSize && 
        ty > 0 && ty <= tileSize && 
        tz > 0 && tz <= tileSize) {
        
        // Convert back to global index within 3D array
        int gi = bx + tx - 1; 
        int gj = by + ty - 1;
        int gk = bz + tz - 1;

        if (gi >= 1 && gi < n-1 && gj >= 1 && gj < n-1 && gk >= 1 && gk < n-1) {
            a[gi][gj][gk] = 0.75f * (
                sharedB[tx-1][ty][tz] + sharedB[tx+1][ty][tz] +  // b[x±1][y][z]
                sharedB[tx][ty-1][tz] + sharedB[tx][ty+1][tz] +  // b[x][y±1][z]
                sharedB[tx][ty][tz-1] + sharedB[tx][ty][tz+1]    // b[x][y][z±1]
            );
        }
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
    dim3 gridDim((n -2 + tileSize - 1) / tileSize, (n - 2+ tileSize - 1) / tileSize, (n - 2+ tileSize - 1) / tileSize);
    dim3 blockDim(tileSize + 2, tileSize + 2, tileSize + 2);

    // track start of operation
    start = CLOCK();

    // Execute kernel
    for(int i = 0; i < 1000; i++){
        nn<<<gridDim, blockDim>>>(cuA, cuB);
    }
    cudaDeviceSynchronize();

    // track end of operation
    end = CLOCK();

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