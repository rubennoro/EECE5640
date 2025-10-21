#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <time.h>

// Global macros for amount of data and number of bins in the histogram
// Vary N from 2^12, 2^18, 2^24
#define N (1 << 12)
#define NUM_BINS 256

// Timer function
double CLOCK() {
	struct timespec t;
	clock_gettime(CLOCK_MONOTONIC,  &t);
	return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

// Random number generation for dataset
void generateData(int* arr){
	for(int i = 0; i < N; i++){
		arr[i] = rand() % 100000 + 1; 
	}
}

int main(){

	srand(time(NULL));

	// Initialize the dataset
	int* data = new int[N];
	generateData(data);
	
	// Initialize the histogram
	int maxVal = 100000;
	int binWidth = (maxVal) / NUM_BINS;
	int mainHistogram[NUM_BINS] = {0};
	
	// Variables for timing
	double start, end, total;

	start = CLOCK();

	// Parallelization with # number of threads on CPU
	#pragma omp parallel
	{
		// Each thread gets an individual histogram
		int privateHist[NUM_BINS] = {0};

		#pragma omp for
		for(int i = 0; i < N; i++){
			// Classifying random values into bins

			int binIdx = (data[i] - 1) / (maxVal / NUM_BINS);
            privateHist[binIdx]++;

		}

		// Adding to main histogram in synch
		#pragma omp critical
        {
            for (int i = 0; i < NUM_BINS; i++) {
                mainHistogram[i] += privateHist[i];
            }
        }

	}
	end = CLOCK();

	total = end - start;

	std::cout << "Results: " << std::endl;

	// Printing an element from each bin
	for(int i = 0; i < NUM_BINS; i++){
		int lower = 1 + i * binWidth;
		int upper = (i == NUM_BINS - 1) ? maxVal : (lower + binWidth - 1);
		// std::cout << "Class " << i+1 << " (" << lower << " ) " << mainHistogram[i] << " elements." << std::endl;
	}
	// Total time
	std::cout << "Time: " << total << std::endl;

	delete[] data;
	return 0;
}
