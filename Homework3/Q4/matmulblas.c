
#include <stdio.h>
#include <cblas.h>
#include <stdlib.h>
#include <time.h>

//Clock function
double CLOCK() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000) + (t.tv_nsec * 1e-6);
}

//Define matrix col/row size
#define N 512

int main(){
        double start, end, total;
        float *A = (float *)malloc(N * N * sizeof(float));
        float *B = (float *)malloc(N * N * sizeof(float));
        float *C = (float *)malloc(N * N * sizeof(float));
        //Set the matrices to values
        int i, j, k;
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        A[i * N + j] = 1;
                        B[i * N + j] = 2;
                        C[i * N + j]= 0;
                }
        }
        /*Commented out due to ineffiency
        //Timing regular matmul
        start = CLOCK();
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        for(k = 0; k < N; k++){
                                C[i * N + j] += A[i * N + k] * B[k * N + j];
                        }
                }
        }
        end = CLOCK();
        total = end - start;
        printf("Regular Time: %f\n", total);
        printf("Regular Val: %f\n", C[0]);
        
        //Reseting C
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        C[i * N + j] = 0;
                }
        }
        */
        //Timing OpenBlas
        start = CLOCK();
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0f, A, N, B, N, 0.0f, C, N);
        end = CLOCK();
        total = end - start;
        printf("OpenBlas Val: %.2f\n", C[0]);
        printf("OpenBlas Time: %.2f\n", total);
        //Free Allocated Memory
        free(A);
        free(B);
        free(C);
        return 0;
}