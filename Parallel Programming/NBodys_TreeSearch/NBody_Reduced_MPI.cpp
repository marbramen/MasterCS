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
#define NProcsTask 4 // number of tasks
#define X_Pos 0
#define Y_Pos 1

int timeFinal = 100, difT = 1;
double constG = 6.673E-3; // 6.673E-11 real value for constant gravity, but is very small almost zero 
// double forces[N][2], pos[N][2], vel[N][2], masses[N];
// double posAux[N][2], velAux[N][2]; // auxiliars vectors for order cyclic distribution
double locVel[N / NProcsTask][2]; // vectors more little for velocitys

double forces[N][2];
double posAux[N][2] = {{-2040.834405, -8551.847208} ,{8131.057051, -8335.860091} ,{1528.236583, 3576.788173} , {-3324.225318, 8501.484002},{503.911819, -2268.822581} ,{-5556.555128, 9441.632861},{-6836.317189, 9752.253778},{3910.929996, -4602.072143},{-1148.571955, -3028.485306},{1803.715477, 1449.352201} ,{930.365432, 623.288830} ,{6367.067935, 8883.980475},{-8395.828269, -8204.61036},{7885.324358, -3247.753509},{7092.929099, 5856.894253},{8536.546686, -4893.826477}};
double velAux[N][2] = {{-29.484060,-11.481},{-42.338259, 36.1740},{29.610747, -38.0100},{-8.421692, 12.19979},{-32.449149, -41.2487},{-38.676806, 8.96544},{6.185724, -13.66564},{-4.399291,27.21639},{31.047645,-26.1321},{-10.645043, -49.403},{1.102438, 22.202678},{9.075899, 43.158534},{2.146612, 14.206582},{22.300942, -14.9726},{-4.164804, 16.51229},{10.399121,-17.9623}}; // vectors 2-dimensions
double masses[N] = {56.684463,99.328216,88.985466,57.648435,61.866451,74.472480,68.640005,56.581852,73.423768,59.723275,12.933867,76.692672,72.655651,55.488812,51.576179,63.063876};
double pos[N][2], vel[N][2];

double locForces[N / NProcsTask][2], tmpForces[N / NProcsTask][2];
double locPos[N / NProcsTask][2], tmpPos[N / NProcsTask][2];


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
		posAux[i][X_Pos] = gRanNum(-10000, 10000);
		posAux[i][Y_Pos] = gRanNum(-10000, 10000);
		velAux[i][X_Pos] = gRanNum(-50, 50);
		velAux[i][Y_Pos] = gRanNum(-50, 50);
		masses[i] = gRanNum(50, 100);		
	}	
}

void orderArraysForCyclicDistribution(int locN, int numberPro){
	for(int i = 0; i < N; i++){
		int rankTemp = i / locN;
		int particle = rankTemp + numberPro * ( i - rankTemp * locN); 		
		pos[particle][X_Pos] = posAux[i][X_Pos];
		pos[particle][Y_Pos] = posAux[i][Y_Pos];
		vel[particle][X_Pos] = velAux[i][X_Pos];
		vel[particle][Y_Pos] = velAux[i][Y_Pos];
	}
}

int firsIndex(int glbPart1, int owner, int rank, int nTask){
	int k = (glbPart1 - owner) / nTask;
	return rank + k * nTask;
}

int globalToLocalIndex(int glbPart2, int owner, int nTask){
	return (glbPart2 - owner) / nTask;
}

int main(int argc, char **argv){
	
	int myRank, locN, numberPro, source, dest, owner;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numberPro);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);	

	locN = N / numberPro;

	MPI_Datatype vectMPIt;
	MPI_Status status;
	MPI_Type_contiguous(2, MPI_DOUBLE, &vectMPIt);
	MPI_Type_commit(&vectMPIt);

	// processor master
	if(myRank == 0){
		//initArrays();	
		printf("Data created by Master processor\n");
		orderArraysForCyclicDistribution(locN, numberPro);		
		printf("Data order by Master processor for cyclic distribution \n");
	}

	source = (myRank + 1) % numberPro;
	dest = (myRank - 1 + numberPro) % numberPro;

	MPI_Bcast(masses, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Scatter(vel, locN, vectMPIt, locVel, locN, vectMPIt, 0, MPI_COMM_WORLD);
	MPI_Scatter(pos, locN, vectMPIt, locPos, locN, vectMPIt, 0, MPI_COMM_WORLD);


	for(int currTime = 0; currTime <= timeFinal; currTime += difT){

		//Gather velocities and forces locals into task 0 for imprention
		MPI_Gather(locVel, locN, vectMPIt, vel, locN, vectMPIt, 0, MPI_COMM_WORLD);
		MPI_Gather(locForces, locN, vectMPIt, forces, locN, vectMPIt, 0, MPI_COMM_WORLD);		

		// Master task prints values		
		if(myRank == 0 ){
			printf("\n\n=============================== TIME %ds ===============\n", currTime);
			for(int i = 0; i < N; i++){		
				int rankTemp = i / locN;
				int particle = rankTemp + numberPro * ( i - rankTemp * locN); 
				printf("Particle %d time %d: Pos(%lf, %lf) Force(%lf, %lf) Velo(%lf, %lf) Mass(%lf)\n", particle,
					currTime, pos[i][X_Pos], pos[i][Y_Pos], time == 0 ? 0.0 : forces[i][X_Pos], time == 0 ? 0.0 : forces[i][Y_Pos], vel[i][X_Pos], vel[i][Y_Pos], masses[particle]); 
			}	
		}

		// PARTICLES LOCALS SUCESSIONS FOR EACH TASKS
		//{Su_Task0} -> {Par0, Part4,  Part8, Part12, ... , Part1596}  
		//{Su_Task1} -> {Par1, Part5,  Part9, Part13, ... , Part1597}  	
		//{Su_Task2} -> {Par2, Part6, Part10, Part14, ... , Part1598}  	
		//{Su_Task3} -> {Par3, Part7, Part11, Part15, ... , Part1599}  	

		// Compute forces among local particles
		// {Su_Taski} x {Su_Taski}
		// convert indexLocal to IndexGlobal, useful for masses array
		for(int i = 0; i < locN; i++){
			locForces[i][X_Pos] = locForces[i][Y_Pos] = 0.0;
			tmpForces[i][X_Pos] = tmpForces[i][Y_Pos] = 0.0;				
		}
		for(int i = 0, glbPart1 = myRank; i < locN; i++, glbPart1 += numberPro){
			double x_dif, y_dif, dist, dist_cubed;		
			double tempForcX, tempForcY;
			tmpPos[i][X_Pos] = locPos[i][X_Pos];
			tmpPos[i][Y_Pos] = locPos[i][Y_Pos];
			for(int j = i + 1, glbPart2 = glbPart1 + numberPro; j < locN; j++, glbPart2 += numberPro){					

				x_dif = locPos[i][X_Pos] - locPos[j][X_Pos];
				y_dif = locPos[i][Y_Pos] - locPos[j][Y_Pos];
				dist = sqrt(x_dif * x_dif + y_dif * y_dif);
				dist_cubed = dist * dist * dist;
				tempForcX = constG * masses[glbPart1] * masses[glbPart2] / dist_cubed * x_dif;
				tempForcY = constG * masses[glbPart1] * masses[glbPart2] / dist_cubed * y_dif;

				// local array  just add values	
				locForces[i][X_Pos] += tempForcX;
				locForces[i][Y_Pos] += tempForcY;				

				// temporal array just for send to others tasks
				tmpForces[j][X_Pos] -= tempForcX;
				tmpForces[j][Y_Pos] -= tempForcY;	
			}	
		}
		
		for(int phase = 1; phase < numberPro; phase++){
			// Send current tempForces and tempPositions to destiny
			// Recieve new tempForces and tempPositions from source 
			MPI_Sendrecv_replace(tmpForces, locN, vectMPIt, dest, 0, source, 0, MPI_COMM_WORLD, &status);
			MPI_Sendrecv_replace(tmpPos, locN, vectMPIt, dest, 0, source, 0, MPI_COMM_WORLD, &status);
			// Owner of the forces we're receiving
			owner = (myRank + phase) % numberPro;

			// Compute forces among local particles for task and owner's particles
			for(int i = 0, glbPart1 = myRank; i < locN - 1; i++, glbPart1 += numberPro){
				double x_dif, y_dif, dist, dist_cubed;
				double tempForcX, tempForcY;
				for(int glbPart2 = firsIndex(glbPart1, owner, myRank, numberPro), 
				 j = globalToLocalIndex(glbPart2, owner, numberPro); 
				 j < locN; j++, glbPart2 += numberPro){

					x_dif = locPos[i][X_Pos] - tmpPos[j][X_Pos];
					y_dif = locPos[i][Y_Pos] - tmpPos[j][Y_Pos];
					dist = sqrt(x_dif * x_dif + y_dif * y_dif);
					dist_cubed = dist * dist * dist;
					tempForcX = constG * masses[glbPart1] * masses[glbPart2] / dist_cubed * x_dif;
					tempForcY = constG * masses[glbPart1] * masses[glbPart2] / dist_cubed * y_dif;

					// local array  just add values	
					locForces[i][X_Pos] += tempForcX;
					locForces[i][Y_Pos] += tempForcY;				

					// temporal array just for send to others tasks
					tmpForces[j][X_Pos] -= tempForcX;
					tmpForces[j][Y_Pos] -= tempForcY;	

				}
			}	
		}
		// Send current tempForces and tempPositions to destiny
		// Recieve new tempForces and tempPositions from source 
		MPI_Sendrecv_replace(tmpForces, locN, vectMPIt, dest, 0, source, 0, MPI_COMM_WORLD, &status);	
		MPI_Sendrecv_replace(tmpPos, locN, vectMPIt, dest, 0, source, 0, MPI_COMM_WORLD, &status);

		//update locForces with that final tempForces calcute 
		for(int i = 0; i < locN; i++){
			locForces[i][X_Pos] += tmpForces[i][X_Pos];
			locForces[i][Y_Pos] += tmpForces[i][Y_Pos];
		}		

		//update velocities and positions
		for(int i = 0, glbPart1 = myRank; i < locN; i++, glbPart1 += numberPro){
			locPos[i][X_Pos] += difT * locVel[i][X_Pos];
			locPos[i][Y_Pos] += difT * locVel[i][Y_Pos];
			locVel[i][X_Pos] += difT * masses[glbPart1] * forces[i][X_Pos];
			locVel[i][Y_Pos] += difT * masses[glbPart1] * forces[i][Y_Pos];
		}
	}

	MPI_Finalize();
	return 0;				
}


