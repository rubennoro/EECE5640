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
//Struct setup to save the thread id, number of hits into circle, and the rand seeding
typedef struct {
  int id;
  int in;
  unsigned int seed;
  int num_tests; //to signify how many tests this thread is supposed to run
} ThreadData;

void* randCoords(void* argp){
  ThreadData* data = (ThreadData*)argp;
  //Randomizes the x and y points
  int threads_hits = 0;
  for(int i = 0; i < data->num_tests; i++){
        float x = ((float)rand_r(&data->seed) / RAND_MAX) * 2.0;
        float y = ((float)rand_r(&data->seed) / RAND_MAX) * 2.0;
        //Calculates whether in circle or not based on euclidean distance outside of radius 1
        float dx = x - 1.0;
        float dy = y - 1.0;
        if ((dx * dx + dy * dy) <= 1.0){
                data->in += 1;
        }
  }
  //printf("%d id number produced coords: (%.2f, %.2f)\n", data->id, x, y);
  return NULL;
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

  //Creating all threads and seeding data with rand
  //Passing threads through the randCoords function
  pthread_t threads[numThreads];
  ThreadData threadData[numThreads];
  int darts_per_thread = pointCount/ numThreads;
  int i;
  double start = CLOCK();
  for(i = 0; i < numThreads; i++){

  threadData[i].id = i;
  threadData[i].in = 0;
  threadData[i].num_tests = darts_per_thread;
  threadData[i].seed = time(NULL) ^ pthread_self() ^ clock() ^ (intptr_t)&threadData[i];
  pthread_create(&threads[i], NULL, randCoords, &threadData[i]);
}

  //This sums the number of ones, which represents the number of inside the circle struct instances from each thread
  //This is then divided by the total number of darts and multiplied by 4 to signify pi
  int totalIn = 0;
  int j;
  for (j = 0; j < numThreads; j++) {
  pthread_join(threads[j], NULL);
  totalIn += threadData[j].in;
}
  double end = CLOCK();
  double t = end - start;
  printf("Time taken: %f ms", t);

  float pi = ((float)totalIn / pointCount) * 4;
  printf("This is pi: %f\n", pi);

  return EXIT_SUCCESS;
}
