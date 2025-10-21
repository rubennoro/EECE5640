#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

// Set the dataset size
#define N (1 << 12)
#define NUM_BINS 256

double CLOCK(){
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec * 1000) + (t.tv_nsec*1e-6);
}

// CUDA Kernel
__global__ void histogram(int* histogram, int* data, int n){

	// Determine index, ensure it's less than n
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    
	if (idx < n) {
        
		// Get value from data and put it in a bin
		int value = data[idx];
        
		int bin = (value - 1) / (100000 / NUM_BINS);
        
		if (bin >= NUM_BINS) bin = NUM_BINS - 1;
		
		//Atomically add it to main histogram
        atomicAdd(&histogram[bin], 1);  // Atomic operation for thread safety
    }
}

// Random number generation for dataset
void generateData(int* arr){
	for(int i = 0; i < N; i++){
		arr[i] = rand() % 100000 + 1; 
	}
}

int main(int argc, char* argv[]){

	srand(time(NULL));

	// Initialize the random numbers into an array
	int* hostData = new int[N];
	generateData(hostData);

	// Initailize histogram values to 0
	int* hostHist = new int[NUM_BINS]();

	int maxVal = 100000;
	int binWidth = (maxVal) / NUM_BINS;

	// CUDA device variables
	int* deviceData;
	int* deviceHist;

	double start, finish, total;

	// Include the device to host communication in timing
	cudaMalloc(&deviceData, N * sizeof(int));
	cudaMalloc(&deviceHist, NUM_BINS * sizeof(int));

	start = CLOCK();
	// Copy arrays over to GPU memory
	cudaMemcpy(deviceHist, hostHist, NUM_BINS * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(deviceData, hostData, N * sizeof(int), cudaMemcpyHostToDevice);

	// Set sizes of parallelization
	int blockSize = 256;  // Optimal for modern GPUs
	int gridSize = (N + blockSize - 1) / blockSize;

	// Execute the kernel
	histogram<<<gridSize, blockSize>>>(deviceHist, deviceData, N);
	cudaDeviceSynchronize();
	
	// Include the host to device communication in timing
	cudaMemcpy(hostHist, deviceHist, NUM_BINS * sizeof(int), cudaMemcpyDeviceToHost);
	
	finish = CLOCK();
	total = finish - start;

	//cudaMemcpy(hostHist, deviceHist, NUM_BINS * sizeof(int), cudaMemcpyDeviceToHost);

	std::cout << "Results: " << std::endl;

	// Print out one element from each class in class ascending order
	for(int i = 0; i < NUM_BINS; i++){
		int lower = 1 + i * binWidth;
		int upper = (i == NUM_BINS - 1) ? maxVal : (lower + binWidth - 1);
		//std::cout << "Class " << i+1 << " (" << lower << " - " << upper << " ) " << hostHist[i] << " elements." << std::endl;
	}
	std::cout << "Time: " << total << std::endl;


	delete[] hostHist;
	delete[] hostData;
    cudaFree(deviceData);
    cudaFree(deviceHist);
	
	return 0;
}
