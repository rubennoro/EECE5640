#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <immintrin.h>
#define N 256        
#define LOOPS 10        

//Global definition of i, j, k, l loops vars
int i, j, k, l;

//Compressed Sparse Row Matrix for Sparse MatMul
typedef struct{


} CSRMat;


double CLOCK() {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC,  &t);
        return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

void regularDense(double a[N][N], double b[N][N], double c[N][N]){
        for (l=0; l<LOOPS; l++) {
                for(i=0; i<N; i++){
                        for(j=0; j<N; j++){
                                c[i][j] = 0.0;
                                for(k=0; k<N; k++){
                                        c[i][j] = c[i][j] + a[i][k] * b[k][j];
                                }
                        }
                }
        }

}

//Using tiling tnd threading o optimize dense matrix multiplication
void optimDense(double a[N][N], double b[N][N], double c[N][N]){
        int ii, jj, kk;

        //Initialize a tile size
        int T = 32;

        //Allocating memory for AVX 
        double (*A)[N] = aligned_alloc(64, N * N * sizeof(double));
        double (*B_tran)[N] = aligned_alloc(64, N * N * sizeof(double));
        double (*C)[N] = aligned_alloc(64, N * N * sizeof(double));

        //double b_tran[N][N];
        //Tranpose B to iterate over rows, reduce cache thrash
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        B_tran[j][i] = b[i][j];
                        A[i][j] = a[i][j];
                }
        }

        for(l=0; l < LOOPS; l++){
                #pragma omp parallel for collapse(2) private(ii, jj, kk, i, j, k)
                for(ii = 0; ii < N; ii += T){
                        for(jj = 0; jj < N; jj+=T){
                                for(i = ii; i < ii + T; i++){
                                        for(j = jj; j < jj + T; j++){
                                                c[i][j] = 0.0;
                                        }
                                }
                                for(kk=0; kk < N; kk+=T){
                                        for(i = ii; i < ii + T && i < N; i++){
                                                for(k = kk; k < kk + T && k < N; k++){
                                                        __m512d a_ind = _mm512_set1_pd(A[i][k]);
                                                        //Process 8 double elements at a time
                                                        for(j = jj; j < jj + T - 7 && j < N - 7; j+=8){
                                                                __m512d b_ind = _mm512_loadu_pd(&B_tran[j][k]);
                                                                __m512d c_ind = _mm512_loadu_pd(&C[i][j]);

                                                                c_ind = _mm512_fmadd_pd(a_ind, b_ind, c_ind);

                                                                _mm512_storeu_pd(&C[i][j], c_ind);

                                                        }
                                                        //Handling the rest of the values
                                                        for(; j < jj + T && j < N; j++){
                                                                C[i][j] += A[i][k] * B_tran[j][k];
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }

        free(A);
        free(B_tran);
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        c[i][j] = C[i][j];
                }
        }
        free(C);

}
//Function for regular sparse matrix multiplication without optimization
void regularSparse(double a[N][N], double b[N][N], double c[N][N]){
        for (l=0; l<LOOPS; l++) {
                for(i=0; i<N; i++){
                        for(j=0; j<N; j++){
                                c[i][j] = 0.0;
                                for(k=0; k<N; k++){
                                        c[i][j] = c[i][j] + a[i][k] * b[k][j];
                                }
                        }
                }
        }

}
void optimSparse(double a[N][N], double b[N][N], double c[N][N]){

}

int main()
{
  double a[N][N]; /* input matrix */
  double b[N][N]; /* input matrix */
  double c[N][N]; /* result matrix */
  int num_zeros;
  double start, finish, total;


  /* initialize a dense matrix */
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      a[i][j] = (double)(i+j);
      b[i][j] = (double)(i-j);
    }
  }

  printf("starting dense matrix multiply \n");
  start = CLOCK();
  regularDense(a, b, c);
  //optimDense(a, b, c);
  finish = CLOCK();
  total = finish-start;
  printf("a result %g \n", c[7][8]); /* prevent dead code elimination */
  printf("The total time for matrix multiplication with dense matrices = %f ms\n", total);

  /* initialize a sparse matrix */
  num_zeros = 0;
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      if ((i<j)&&(i%2>0))
        {
         a[i][j] = (double)(i+j);
         b[i][j] = (double)(i-j);
        }
      else
        {
         num_zeros++;
         a[i][j] = 0.0;
         b[i][j] = 0.0;
        }

     }
  }

  printf("starting sparse matrix multiply \n");
  start = CLOCK();
  regularSparse(a, b, c);

  finish = CLOCK();
  total = finish-start;
  printf("A result %g \n", c[7][8]); /* prevent dead code elimination */
  printf("The total time for matrix multiplication with sparse matrices = %f ms\n", total);
  printf("The sparsity of the a and b matrices = %f \n", (float)num_zeros/(float)(N*N));


return 0;
}

