#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

double CLOCK() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC,  &t);
  return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

//Structure to store the sum of each thread's calculations, and to track number of values for each thread
typedef struct{
  int num_vals;
  double sum;
  int start;
}LeibThread;

//Function for each thread's calculation
void* LeibCalc(void* argp){
  //Threaded sum
  LeibThread* calculation = (LeibThread*)argp;

  for(int i = calculation->start; i < calculation->start + calculation->num_vals; i++){
        //Leibniz Summation
        calculation->sum += (pow(-1, i) / (2 * i + 1));
  }

}

int main(int argc, char* argv[]){

  //Input the number of calculations and number of threads when running the program
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <number_of_points> <num_threads> \n", argv[0]);
    return EXIT_FAILURE;
  }
  //pointCount stores nums in series, comes first
  int pointCount = atoi(argv[1]);
  if (pointCount <= 0) {
    fprintf(stderr, "Number of points must be positive\n");
    return EXIT_FAILURE;
  }
  //numThreads comes next
  int numThreads = atoi(argv[2]);

  //Condition for if there's less points inputted than threads
  //If this is the case, set the pointCount to the number of threads
  if(pointCount < numThreads){
          pointCount = numThreads;
  }
  pthread_t threads[numThreads];
  LeibThread threadCalc[numThreads];

  int calc_per_thread = pointCount / numThreads;

  double start = CLOCK();
  for(int i = 0; i < numThreads; i++){
    threadCalc[i].num_vals = calc_per_thread;
    //This calculation helps determine where the thread will start
    //Each thread will sum one new value every # of threads
    threadCalc[i].start = i * calc_per_thread;
    pthread_create(&threads[i], NULL, LeibCalc, &threadCalc[i]);

  }
  //Calculating the total sum of all threads
  double total = 0;
  //Killing all threads and summing all values
  for(int i = 0; i < numThreads; i++){
    pthread_join(threads[i], NULL);
    total += threadCalc[i].sum;
  }

  //Time calculation
  double end = CLOCK();
  double time = end - start;
  printf("Time taken: %f ms\n", time);

  //Multiplying sum by 4 to find Leibniz approximation
  double pi = 4 * total;
  printf("Pi Approx: %f \n", pi);


  return 0;

}
~           