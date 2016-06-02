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

#define N 1600 // number of particles
#define NProcsTask 8 // number of tasks
#define X_Pos 0
#define Y_Pos 1

int timeFinal = 100, difT = 1;
double constG = 6.673E-3; // 6.673E-11 real value for constant gravity, but is very small almost zero 
double forces[N][2], pos[N][2], vel[N][2], masses[N];
double locVel[N / NProcsTask][2]; // vectors more little for velocitys


double gRanNum(double min, double max){
	double res = (max -  min) * ((double)rand() / (double)RAND_MAX) + min;
	// For avoiding velocity equal 0 because 	its acceleration had been 0
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
		vel[i][X_Pos] = gRanNum(-50, 50);
		vel[i][Y_Pos] = gRanNum(-50, 50);
		masses[i] = gRanNum(50, 100);		
	}
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
	MPI_Bcast(pos, N , vectMPIt, 0, MPI_COMM_WORLD);
	MPI_Scatter(vel, locN, vectMPIt, locVel, locN, vectMPIt, 0, MPI_COMM_WORLD);

	for(int currTime = 0; currTime < timeFinal; currTime += difT){

		//Gather velocities and forces locals into task 0 for imprention
		MPI_Gather(locVel, locN, vectMPIt, vel, locN, vectMPIt, 0, MPI_COMM_WORLD);
		MPI_Gather(forces, locN, vectMPIt, forces, locN, vectMPIt, 0, MPI_COMM_WORLD);

		// Task master print values		
		if(myRank == 0 ){
			printf("\n\n=============================== TIME %ds ===============\n", currTime);
			for(int i = 0; i < N; i++){		
				printf("Particle %d time %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", i,
					currTime, pos[i][X_Pos], pos[i][Y_Pos], forces[i][X_Pos], forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[i]); 
			}	
		}

		// Initialize local arrays force
		// this be better inside next for
		// but the others codes have the same
		for(int i = 0; i < locN; i++){
			forces[i][X_Pos] = 0;
			forces[i][Y_Pos] = 0;
		}

		// Calculate values just for the block of particles have each task
		// star: myRank * locN
		// end: myRank * locN + locN
		// set values in the forces array begin index 0 to index locN		
		for(int i = myRank * locN; i < (myRank +1)* locN; i++){
			double x_dif, y_dif, dist, dist_cubed;			
			for(int j = 0; j < N; j++){
				if(i != j){
					x_dif = pos[i][X_Pos] - pos[j][X_Pos];
					y_dif = pos[i][Y_Pos] - pos[j][Y_Pos];
					dist = sqrt(x_dif * x_dif + y_dif * y_dif);
					dist_cubed = dist * dist * dist;
					forces[i - myRank * locN][X_Pos] -= constG * masses[i] * masses[j] / dist_cubed * x_dif;
					forces[i - myRank * locN][Y_Pos] -= constG * masses[i] * masses[j] / dist_cubed * y_dif;
				}
			}
		}

		// Compute new values for velocites and positions
		// Be careful with the velocities, positions, masses touches to every tas
		// Compute positions new in index 0 to locN, after  
		for(int i = 0; i < locN; i++){
			pos[i][X_Pos] = pos[i + myRank * locN][X_Pos] + difT * locVel[i][X_Pos];
			pos[i][Y_Pos] = pos[i + myRank * locN][Y_Pos] + difT * locVel[i][Y_Pos];
			locVel[i][X_Pos] += difT * masses[i + myRank * locN] * forces[i][X_Pos];
			locVel[i][Y_Pos] += difT * masses[i + myRank * locN] * forces[i][Y_Pos];	
		}

		//Allgather local positions(index 0 to index locN)for each task into global pos array	
		MPI_Allgather(pos, locN, vectMPIt, pos, locN, vectMPIt, MPI_COMM_WORLD);

	} 
	
	MPI_Gather(locVel, locN, vectMPIt, vel, locN, vectMPIt, 0, MPI_COMM_WORLD);
	MPI_Gather(forces, locN, vectMPIt, forces, locN, vectMPIt, 0, MPI_COMM_WORLD);

	// Task master print values		
	if(myRank == 0 ){
		printf("\n\n=============================== TIME %ds ===============\n", timeFinal);
		for(int i = 0; i < N; i++){		
			printf("Particle %d time %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", i,
				timeFinal, pos[i][X_Pos], pos[i][Y_Pos], forces[i][X_Pos], forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[i]); 
		}	
	}

	MPI_Finalize();
	return 0;
	
}

