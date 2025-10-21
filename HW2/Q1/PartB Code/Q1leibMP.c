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

double Leibniz(int points, int threads){
  double sum = 0;

  //OpenMP directive will add up the thread sums and use the input amount of threads
  #pragma omp parallel reduction(+ : sum) num_threads(threads)
  {
    //In case there are leftover points not evenly distributed amongst threads
    int thread_points = points / threads;
    if(omp_get_thread_num() < points % threads){
           thread_points++;
    }
    //Dividing up the work evenly
    int start = omp_get_thread_num() * (points / threads) + (omp_get_thread_num() < points % threads ? omp_get_thread_num() : points % threads);
    for(int i = start; i < thread_points + start; i++){
      sum+= (pow(-1, i) / (2 * i + 1));
    }
  }
  return 4 * sum;
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
  double start = CLOCK();
  double pi = Leibniz(pointCount, numThreads);
  double end = CLOCK();
  double time = end - start;
  printf("Time taken: %f ms\n", time);
  printf("Pi Approx: %f \n", pi);
  return EXIT_SUCCESS;
}