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
#define N 1600 // number of particles
#define X_Pos 0
#define Y_Pos 1

double forces[N][2], pos[N][2], vel[N][2]; // vectors 2-dimensions
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
		forces[i][X_Pos] = 0; // this is redundant
		forces[i][Y_Pos] = 0; // this is redundant
		pos[i][X_Pos] = gRanNum(-10000, 10000);
		pos[i][Y_Pos] = gRanNum(-10000, 10000);
		vel[i][X_Pos] = gRanNum(-500, 500);
		vel[i][Y_Pos] = gRanNum(-500, 500);
		masses[i] = gRanNum(50, 100);		
	}
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
	int timeFinal = 1000, difT = 1;
	double constG = 6.673E-3; // 6.673E-11 real value for constant gravity, but is very small almost zero 

	clock_t time = clock();	
	for(int currTime = 0; currTime <= timeFinal; currTime += difT){
		
		//Section for print values
		printf("\n\n======== TIME %ds ====== \n\n", currTime);
		for(int i = 0; i < N; i++)
		{
			printf("Particle %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", 
				i + 1, pos[i][X_Pos], pos[i][Y_Pos], forces[i][X_Pos], forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[i]); 
		}
			
		for(int i = 0; i < N; i++){
			forces[i][X_Pos] = 0;
			forces[i][Y_Pos] = 0;
		}

		for(int	i = 0; i < N; i++){
			double x_dif, y_dif, dist, dist_cubed;		
			double tempForcX, tempForcY;
			for(int j = i + 1; j < N; j++){
				if( i != j){
					x_dif = pos[i][X_Pos] - pos[j][X_Pos];
					y_dif = pos[i][Y_Pos] - pos[j][Y_Pos];
					dist = sqrt(x_dif * x_dif + y_dif * y_dif);
					dist_cubed = dist * dist * dist;
					tempForcX = constG * masses[i] * masses[j] / dist_cubed * x_dif;
					tempForcY = constG * masses[i] * masses[j] / dist_cubed * y_dif;
					forces[i][X_Pos] += tempForcX;
					forces[i][Y_Pos] += tempForcY;				
					forces[j][X_Pos] -= tempForcX;
					forces[j][Y_Pos] -= tempForcY;
				}
			}
		}		
		
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
