#include <stdio.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <time.h>

#define N 512

double CLOCK() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000) + (t.tv_nsec * 1e-6);
}

int main(int argc, char* argv[]){

	double start, finish;
	float a[N][N], b[N][N], c[N][N];

	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			a[i][j] = 1;
			b[i][j] = 2;
			c[i][j] = 0;
		}
	}
	
	start = CLOCK();
	int k;
	for(i = 0; i < N; i++){
	       for(j = 0; j < N; j++){
			for(k = 0; k < N; k++){
				c[i][j] += a[i][k] * b[k][j]; 
			}	
	       }
	}	       

	finish = CLOCK();
	double total = finish - start;
	printf("No AVX Time : %f\n", total);
	printf("No AVX Result: %f\n", c[0][0]);

	for(i = 0; i < N; i++){
	       for(j = 0; j < N; j++){
	       		c[i][j] = 0;
	       }
	}	       
	start = CLOCK();

	for(i = 0; i < N; i++){
		for(j = 0; j < N; j+=16){
			//Accumulator Initialization
			__m512 cc = _mm512_setzero_ps();
			for(k = 0; k < N; k++){
				__m512 aa = _mm512_set1_ps(a[i][k]);
				__m512 bb = _mm512_loadu_ps(&b[k][j]);
				cc = _mm512_fmadd_ps(aa, bb, cc); 
			}
			_mm512_storeu_ps(&c[i][j], cc);
		}
	}

	finish = CLOCK();
	total = finish - start;
	printf("AVX Time: %f\n", total);
	printf("AVX Result: %f\n", c[0][0]);

	return 0;
}
