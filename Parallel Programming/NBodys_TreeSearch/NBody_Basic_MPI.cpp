/*
 HOMEWORK 20160525
 Marchelo Bragagnini
.. N-Body Problem
.. basic solution
.. using MPI
*/
#include<mpi.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>

#define NUM_PR 4 // number of processors
#define N 16 // number of particles
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
		forces[i][X_Pos] = 0;
		forces[i][Y_Pos] = 0;	
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

int main(int argc, char **argv){
	
	int myRank, locN, numberPro;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberPro);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);	

	locN = N / numberPro;

	MPI_Datatype vectMPIt;
	MPI_Type_contiguous(2, MPI_DOUBLE, &vectMPIt);
	MPI_Type_commit(&vectMPIt);

	// processor master
	if(myRank == 0){
		initArrays();
		printf("Data created by Master processor\n");
	}
	MPI_Bcast(masses, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(pos, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(vel, locN, vectMPIt, vel, locN, vectMPIt, 0, MPI_COMM_WORLD);

	printf("\n ===========MyRank %d\n\n", myRank);
	prVecMss();

	MPI_Finalize();
	return 0;
}
