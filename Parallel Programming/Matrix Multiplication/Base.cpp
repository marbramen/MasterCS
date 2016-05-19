// TAREA MCC-PP-TA01 20160511 
#include<cstdio>
#include<string.h>
#include<time.h>

#define N 1600

int A[N][N], B[N][N], C[N][N], sum;

int main(){
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	memset(C, 0, sizeof(C));
	clock_t time = clock();	
	for(int i = 0; i < N; i++)
		for(int j = 0; j < N; j++){
			sum = A[i][j];
			for(int k = 0; k < N; k++)
				sum += B[i][k] * C[k][j];
			A[i][j] = sum; 
	}
	printf("Tiempo ==> %f s.\n", ((float)(clock() - time))/CLOCKS_PER_SEC);
	return 0;
}
