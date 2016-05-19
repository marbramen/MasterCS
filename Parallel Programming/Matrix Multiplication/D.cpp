// HOMEWORK MCC-PP-TA01 20160511 
#include<cstdio>
#include<string.h>
#include<time.h>

#define N 3200
#define BLOCK_SIZE 32

int A[N][N], B[N][N], C[N][N], sum;

void multAB(){
	for(int k = 0; k < N; k+= BLOCK_SIZE )
	  for(int j = 0; j < N; j += BLOCK_SIZE)
	    for(int i2 = 0; i2 < N; i2++)
	      for(int j2 = j; j2 < j + BLOCK_SIZE; j2++){
		sum = A[i2][j2];	
		for(int k2 = k; k2 < k + BLOCK_SIZE; k2++)	
				sum += B[i2][k2] * C[k2][j2];
		A[i2][j2] = sum;
	      }
}

int main(){
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	memset(C, 0, sizeof(C));
	clock_t time = clock();	
	multAB();	
	time = clock() - time;	
	printf("Time ==> %f s.\n", ((float)(time))/CLOCKS_PER_SEC);
	return 0;
}
