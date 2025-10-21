
#include <stdio.h>
#include <iostream>
#include <mpi.h>
#include <vector>
//Histograms splitting data range into equal-sized bins
//Each bin, total number of data values in the data set in it
//Input range 1-100,000 numbers at random
//Dataset 1 contains 8 million #'s
//Vary the number of bins
//Assign bins to MPI processes
#define DATA 8000000
#define BINS 32
#define MAX_RAND 100000

//Function to generate the random data of 8 million points from 1 - 100,000
void generateData(std::vector<int>& arr){
        for(int i = 0; i < DATA; i++){
                arr[i] = rand() % MAX_RAND + 1;
        }
}
//Takes data and separates it into bins for the histogram, from 1 to 32
void getHistogram(const std::vector<int>& local_data, std::vector<int>& hist){
        for(auto& val : local_data){
                int bin = (val - 1) / (MAX_RAND / BINS);
                hist[bin]++;
        }
}

int main(int argc, char** argv){
        //Initialize MPI
        MPI_Init(&argc, &argv);

        int rank, size;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        
        //Time vals
        double start, end, total;
        
        //Initialize vector
        std::vector<int> data(DATA, 0);
        
        //Use head node to generate data
        if(rank == 0){
                //start = MPI_Wtime();
                //Intialized randomness
                srand(time(NULL));
                generateData(data);
        }
        
        //Determine the number of points per bin
        int chunk_size = DATA / size;
        start = MPI_Wtime();
        
        //Local data collection
        std::vector<int> local_bins(chunk_size);
        //Distribute chunks of array to all other processes
        MPI_Scatter(data.data(), chunk_size, MPI_INT, local_bins.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
        
        //Local process histograms
        std::vector<int> local_hist(BINS, 0);
        
        //create histogram
        getHistogram(local_bins, local_hist);
        
        std::vector<int> full_hist(BINS, 0);
        
        //reduce local histograms into main one
        MPI_Reduce(local_hist.data(), full_hist.data(), BINS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        //Calculate time at head process
        if(rank == 0){
                end = MPI_Wtime();
                total = end - start;
                for(int i = 0; i < BINS; i++){
                        std::cout << "Bin " << i << ": " << full_hist[i] << " values " << std::endl;
                }
                std::cout << "Time: " << total << std::endl;
        }

        //Finalize MPI
        MPI_Finalize();
        return 0;
}