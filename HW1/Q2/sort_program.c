
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

//Varied between 1, 2, 4, 8, and 32
//#define THREADS 32

#define ARR_SIZE 10000

//Array to be Sorted
int arr[ARR_SIZE];

//Time Function
double CLOCK() {
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC,  &t);
  return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

void merge(int l, int m, int r){
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int L[n1], R[n2];

    // Copy data to temp arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temp arrays back into arr[l..r
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[],
    // if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[],
    // if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int l, int r)
{
    int m = l + (r - l) / 2;
    if (l < r) {

        //Continuously simplify array until it can be merged in a sorted fashion
        mergeSort(l, m);
        mergeSort(m + 1, r);

        //Sort the merged array
        merge(l, m, r);
    }
}


void* multi_sort(void* arg){
  int id = *(int*)arg;

  int l = id * (ARR_SIZE / THREADS);
  int r = (id + 1) * (ARR_SIZE / THREADS) -1;
  if(id == THREADS-1){
    r = ARR_SIZE - 1;
  }

  mergeSort(l, r);

}

int main(){

  //Initialize variables for time
  double start, end, t;

  //Random # Generation to be inserted into Array
  //Numbers from 1 to 20,0000
  srand(time(NULL));
  int random = rand();

  int i;
  for(i = 0; i < ARR_SIZE; i++){
    random = rand() % (10000) + 1;
    arr[i] = random;
    //printf(" %d\n", array[i]);
  }

  pthread_t threads[THREADS];

  int thread_ids[THREADS];
  start = CLOCK();

  //Create threads and pass unique IDs to the multi_sort function to partition the array
  for(i = 0; i < THREADS; i++){
    thread_ids[i]= i;
    pthread_create(&threads[i], NULL, multi_sort, &thread_ids[i]);
  }
  //Join threads to end
  for(i = 0; i < THREADS; i++){
    pthread_join(threads[i], NULL);
  }

  int step;
  for(step = ARR_SIZE / THREADS; step < ARR_SIZE; step *= 2){
    for(i = 0; i < ARR_SIZE; i += 2 * step){
      int l = i;
      int m = i + step - 1;
      int r;
      if(i + 2 * step - 1< ARR_SIZE){
        r = i + 2 * step - 1;
      }else{
        r = ARR_SIZE - 1;
      }
      if (m < ARR_SIZE && m + 1 < r) {
          merge(l, m, r);
      }
    }
  }
  end = CLOCK();
  t = end - start;
  printf("Number of threads: %d\n", THREADS);
  printf("%f ms\n", t);
  //int j;
  //for(j = 0; j < ARR_SIZE; j++){
  //  printf(" %d\n", arr[j]);
  //}
  return 0;
}