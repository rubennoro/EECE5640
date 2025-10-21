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
int main(int argc, char *argv[]){
        if (argc != 2) {
                fprintf(stderr, "Usage: %s <number_of_points>\n", argv[0]);
                return EXIT_FAILURE;
        }

        int pointCount = atoi(argv[1]);
        if (pointCount <= 0) {
                fprintf(stderr, "Number of points must be positive\n");
                return EXIT_FAILURE;
        }
        double start = CLOCK();
        int i = 0;
        int hits = 0;
        for(i = 0; i < pointCount; i++){
                unsigned int seed = time(NULL) ^ clock() ^ i;
                float x = ((float)rand_r(&seed) / RAND_MAX) * 2.0;
                float y = ((float)rand_r(&seed) / RAND_MAX) * 2.0;
                //Calculates whether in circle or not based on euclidean distance outside of radius 1
                float dx = x - 1.0;
                float dy = y - 1.0;
                if ((dx * dx + dy * dy) <= 1.0){
                        hits += 1;
                }

        }
        double end = CLOCK();
        double t = end - start;
        printf("Time taken: %f ms", t);
        float pi = (hits / (float)pointCount) * 4;
        printf("Pi: %f", pi);
        return 0;
}