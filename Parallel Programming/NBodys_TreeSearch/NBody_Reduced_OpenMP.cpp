/*
 HOMEWORK 20160525
 Marchelo Bragagnini
.. N-Body Problem
.. basic solution
.. using OpenMP
*/
#include<omp.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define NUM_PR 4 // number of processors
#define NUM_THREADS 80
#define N 1600 // number of particles
#define X_Pos 0
#define Y_Pos 1

double forces[N][2], pos[N][2], vel[N][2]; // vectors 2-dimensions
double loc_forces[NUM_THREADS][N][2]; // for the forces calculates for each thread
double masses[N]; // mass is constant just for this example

double gRanNum(double min, double max){
	double res = (max -  min) * ((double)rand() / (double)RAND_MAX) + min;
	// For avoiding velocity equal 0 because its acceleration had been 0
	// For avoiding mass equal 0 because the object don't exist
	if( res == 0.0) return gRanNum(min, max);
	return res;
}

void initArrays(){
	srand(time(NULL));
	for(int i = 0; i < N; i++){		
		pos[i][X_Pos] = gRanNum(-10000, 10000);
		pos[i][Y_Pos] = gRanNum(-10000, 10000);
		vel[i][X_Pos] = gRanNum(-500, 500);
		vel[i][Y_Pos] = gRanNum(-500, 500);
		masses[i] = gRanNum(50, 100);		
	}
	memset(forces, 0, sizeof(forces));
	memset(loc_forces, 0, sizeof(loc_forces));
}

void prVecMss(){
	for(int i = 0; i < N; i++)
		printf("Particle %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", 
		i + 1, pos[i][X_Pos], pos[i][Y_Pos], forces[i][X_Pos], forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[i]); 
}

int main(){
	initArrays();		
	printf("Data created\n");
	//prVecMss();
	int timeFinal = 100, difT = 1;
	double constG = 6.673E-3; // 6.673E-11 real value for constant gravity, but is very small almost zero 

	omp_set_num_threads(NUM_THREADS);

	clock_t time = clock();	

	#pragma omp parallel 
	for(int currTime = 0; currTime <= timeFinal; currTime += difT){
		
		//Section for print values
		#pragma omp single
		{
			printf("\n\n======== TIME %ds ====== \n\n", currTime);		
			for(int i = 0; i < N; i++)
			{
				printf("Particle %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", 
					 i + 1, pos[i][X_Pos], pos[i][Y_Pos], forces[i][X_Pos], forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[i]); 
			}
		}	
		
		#pragma omp for schedule(static, N / NUM_THREADS)
		for(int j = 0; j < NUM_THREADS; j++){
			for(int i = 0; i < N; i++){
				loc_forces[j][i][X_Pos] = 0;
				loc_forces[j][i][Y_Pos] = 0;
			}
		}

		//FIRST PHASE - form ciclic partition
		#pragma omp for
		for(int	i = 0; i < N; i++){
			double x_dif, y_dif, dist, dist_cubed;		
			double tempForcX, tempForcY;
			int myRank = omp_get_thread_num();
			for(int j = i + 1; j < N; j++){			
				x_dif = pos[i][X_Pos] - pos[j][X_Pos];
				y_dif = pos[i][Y_Pos] - pos[j][Y_Pos];
				dist = sqrt(x_dif * x_dif + y_dif * y_dif);
				dist_cubed = dist * dist * dist;				
				tempForcX = constG * masses[i] * masses[j] / dist_cubed * x_dif;
				tempForcY = constG * masses[i] * masses[j] / dist_cubed * y_dif;

				loc_forces[myRank][i][X_Pos] += tempForcX;
				loc_forces[myRank][i][Y_Pos] += tempForcY;				
				loc_forces[myRank][j][X_Pos] -= tempForcX;
				loc_forces[myRank][j][Y_Pos] -= tempForcY;				
			}
		}			
		
		//SECOND PHASE - no matter if partition block
		#pragma omp for schedule(static, N / NUM_THREADS)
		for(int i = 0; i < N; i++){
			forces[i][X_Pos] = 0;
			forces[i][Y_Pos] = 0;
			for(int j = 0; j < NUM_THREADS; j++){
				forces[i][X_Pos] += loc_forces[j][i][X_Pos];
				forces[i][Y_Pos] += loc_forces[j][i][Y_Pos];
			}
		}

		#pragma omp for schedule(static, N / NUM_THREADS)
		for(int i = 0; i < N; i++){
			pos[i][X_Pos] += difT * vel[i][X_Pos];
			pos[i][Y_Pos] += difT * vel[i][Y_Pos];
			vel[i][X_Pos] += difT * masses[i] * forces[i][X_Pos];
			vel[i][Y_Pos] += difT * masses[i] * forces[i][Y_Pos];
		}	

	}
	time = clock() - time;		
	printf("Total time simulation  ==> %f s.\n", ((float)(time))/CLOCKS_PER_SEC);
	return 0;
}
