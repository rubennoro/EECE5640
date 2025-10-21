#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <immintrin.h>
#define N 512
#define LOOPS 10

//Global definition of i, j, k, l loops vars
int i, j, k, l;


double CLOCK() {
        struct timespec t;
        clock_gettime(CLOCK_MONOTONIC,  &t);
        return (t.tv_sec * 1000)+(t.tv_nsec*1e-6);
}

//Function for regular dense matrix multiplication
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

//Using tiling, AVX, and openmp to optimize dense matrix multiplication
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
                //OpenMP pragma
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
                                                //Using AVX512
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
        //Saving result values before freeing memory
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

//Struct to store relevant matrix information for sparse matrix
typedef struct{
        int num_non_zero;
        //Stores the number of non-zero elements in the matrix
        double* A;
        //Stores the cumulative number of non-zero elements upto ith row
        int* IA;
        //Stores column index of each element in A vector
        int* JA;


}CSRMat;

CSRMat* getCSR(double mat[N][N]){
        int nnz = 0;
        for(i = 0; i < N; i ++){
                for(j = 0; j < N; j++){
                        if(mat[i][j] != 0.0){
                                nnz+=1;
                        }
                }
        }

        //Allocating memory for CSR structure

        CSRMat* csr = (CSRMat*)malloc(sizeof(CSRMat));
        if (csr == NULL) {
                fprintf(stderr, "Memory allocation failed for CSRMat\n");
                exit(1);
        }

        csr->num_non_zero = nnz;
        csr->IA = (int*)malloc(sizeof(int) * (N+1));
        csr->A = (double*)malloc(sizeof(double) * nnz);
        csr->JA = (int*)malloc(sizeof(int) * nnz);
        csr->IA[0] = 0;
        //printf("Here1");
        int idx = 0;
        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        if(mat[i][j] != 0.0){
                                csr->A[idx] = mat[i][j];
                                csr->JA[idx] = j;
                                idx += 1;
                        }
                }
                csr->IA[i+1] = idx;
        }

        return csr;
}

void freeCSR(CSRMat* csr){
        free(csr->A);
        free(csr->IA);
        free(csr->JA);
        free(csr);
}
void optimSparse(double a[N][N], double b[N][N], double c[N][N]){
        CSRMat* csrA = getCSR(a);
        CSRMat* csrB = getCSR(b);

        for(i = 0; i < N; i++){
                for(j = 0; j < N; j++){
                        c[i][j] = 0.0;
                }
        }
        //#pragma omp parallel for
        for(i = 0; i < N; i++){
                int aIDX, k, bIDX, j;
                double aVal, bVal;
                for(aIDX = csrA->IA[i]; aIDX < csrA->IA[i+1]; aIDX++){
                        k = csrA->JA[aIDX];
                        aVal = csrA->A[aIDX];

                        for(bIDX = csrB->IA[k]; bIDX < csrB->IA[k+1]; bIDX++){
                                j = csrB->JA[bIDX];
                                bVal = csrB->A[bIDX];

                                c[i][j] += aVal * bVal;
                        }
                }
        }
        freeCSR(csrA);
        freeCSR(csrB);
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

  //Choose to either run optimized version or regular version
  //regularDense(a, b, c);
  optimDense(a, b, c);

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
  
  //Choose to either run optimized version or regular version
  //regularSparse(a, b, c);
  optimSparse(a, b, c);

  finish = CLOCK();
  total = finish-start;
  printf("A result %g \n", c[7][8]); /* prevent dead code elimination */
  printf("The total time for matrix multiplication with sparse matrices = %f ms\n", total);
  printf("The sparsity of the a and b matrices = %f \n", (float)num_zeros/(float)(N*N));


return 0;
}