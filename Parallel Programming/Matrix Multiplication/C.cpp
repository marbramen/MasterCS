// HOMEWORK MCC-PP-TA01 20160511 
#include <stdio.h>
#include<string.h>
#include <time.h>

#define N 3200
#define BLOCK_SIZE 32

int A[N][N], B[N][N], C[N][N];
int i, i2, j, j2, k, k2;
int *rres, *rmul1, *rmul2;

void multAB(){
	for (i = 0; i < N; i += BLOCK_SIZE)
	  for (j = 0; j < N; j += BLOCK_SIZE)
	    for (k = 0; k < N; k += BLOCK_SIZE)
	      for (i2 = 0, rres = &A[i][j], rmul1 = &B[i][k]; i2 < BLOCK_SIZE; ++i2, rres += N, rmul1 += N){
		for (k2 = 0, rmul2 = &C[k][j]; k2 < BLOCK_SIZE; ++k2, rmul2 += N){
		  for (j2 = 0; j2 < BLOCK_SIZE; j2 += 2){
		     rres[j2] += rmul1[k2] * rmul2[j2];
		  }
	        }
	      }
}

int main (void)
{
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	memset(C, 0, sizeof(C));
	clock_t time = clock();
	multAB();
	time = clock() - time;	
	printf("Time ==> %f s.\n", ((float)(time))/CLOCKS_PER_SEC);
	return 0;
}
