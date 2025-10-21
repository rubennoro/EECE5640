#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

double CLOCK() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC,  &t);
  return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

double monteCarlo(int points, int threads){
  int hits = 0;

  //Sum all the thread states of hits
  #pragma omp parallel reduction(+ : hits) num_threads(threads)
  {
    //Each thread will run on points/threads number of points to ensure that the work is divided evenly
    //Random seeding
    unsigned int rand_seed = omp_get_thread_num();
    int thread_points = points / threads;
    if(omp_get_thread_num() < points % threads){
           thread_points++;
    }
    for(int i = 0; i < thread_points; i++){
        float x = ((float)rand_r(&rand_seed) / RAND_MAX) * 2.0;
        float y = ((float)rand_r(&rand_seed) / RAND_MAX) * 2.0;

        float dx = x - 1.0;
        float dy = y - 1.0;
        if((dx * dx + dy * dy) <= 1.0){
          hits++;
        }
     }
  }

  //Calculation for pi here
  return 4 * ((double)hits/(double)points);
}

int main(int argc, char *argv[]){
  //Input the number of darts and number of threads when running the program
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <number_of_points> <num_threads> \n", argv[0]);
    return EXIT_FAILURE;
  }

  int pointCount = atoi(argv[1]);
  if (pointCount <= 0) {
    fprintf(stderr, "Number of points must be positive\n");
    return EXIT_FAILURE;
  }
  int numThreads = atoi(argv[2]);
  //Condition for if there's less points inputted than threads
  //If this is the case, set the pointCount to the number of threads
  if(pointCount < numThreads){
          pointCount = numThreads;
  }
  //Calculating time
  double start = CLOCK();
  double pi = monteCarlo(pointCount, numThreads);
  double end = CLOCK();
  double time = end - start;

  //Printing results
  printf("Time taken: %f ms\n", time);
  printf("Pi Approx: %f \n", pi);
  return EXIT_SUCCESS;
}
