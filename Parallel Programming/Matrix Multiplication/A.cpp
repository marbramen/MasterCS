// HOMEWORK MCC-PP-TA01 20160511 
#include<cstdio>
#include<string.h>
#include<time.h>

#define N 3200

int A[N][N], B[N][N], C[N][N], b;

void multAB(){
	for(int i = 0; i < N; i++)
	  for(int k = 0; k < N; k++){
	    b = B[i][k];	
	    for(int j = 0; j < N; j++)
	      // SAPXY Operation
	      // A = b * X + Y
	      A[i][j] += b * C[k][j];
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
