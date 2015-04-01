#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h> 

int M;
int p;
int s;

int getRowCoordinate(int pid, int size) { //Change 1D numbering of Processors into 2D
	return pid / size;
}

int getColCoordinate(int pid, int size) { //Change 1D numbering of Processors into 2D
	return pid % size;
}

void printMatrix(int** m, int rows, int cols) { //Print a matrix of size ( row X cols ) to the screen
	int i, j;
	
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			printf("%d\t ", m[i][j]);
		}
		printf("\n");
	}
}

void multiply(int **A, int **B, int** C, int m, int size) { //Serially Multiply A x B and put result in C
	int i, j, k;
	
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			C[i][j] = 0;
			
			for (k = 0; k < m; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

int main(int argc, char ** argv) { //Main
	MPI_Init(&argc, &argv);

	int i, j, size, row, col, sqp, tmp;
	int **A, **B, **C;
	double t0, t1;

	srand(time(NULL));
	sscanf(argv[1], "%d", &M); //Read matrix dimension
	
	MPI_Comm_size(MPI_COMM_WORLD, &p); //get number of processors.
	MPI_Comm_rank(MPI_COMM_WORLD, &s); //get processor id.
	
	sqp = sqrt(p);
	size = M/sqp;
	
	if(s == 0) { //Initialize arrays
		A = (int**) malloc(sizeof(int*) * M);
		B = (int**) malloc(sizeof(int*) * M);
		C = (int**) malloc(sizeof(int*) * M);
		
		for(i = 0;i < M;i++) {
			A[i] = (int*) malloc(sizeof(int) * M);
			B[i] = (int*) malloc(sizeof(int) * M);
			C[i] = (int*) malloc(sizeof(int) * M);
			
			for(j = 0;j < M;j++) {
				A[i][j] = rand() % 5;
				B[i][j] = rand() % 5;
			}	
		}
		
		//printf("A: \n");
		//printMatrix(A, M, M);
	
		//printf("\nB: \n");
		//printMatrix(B, M, M);
	}
	
	int **localA, **localB, **localC; //local Arrays
	
	localA = (int**) malloc(sizeof(int*) * size);
	localB = (int**) malloc(sizeof(int*) * M);
	localC = (int**) malloc(sizeof(int*) * size);
	
	for(i = 0;i < M;i++) {
		localB[i] = (int*) malloc(sizeof(int) * size);
		
		if(i < size) {
			localA[i] = (int*) malloc(sizeof(int) * M);
			localC[i] = (int*) malloc(sizeof(int) * size);
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD); //sync
	
	t0 = MPI_Wtime();

	if(s == 0) {
		for(i = 0; i < p; i++) { //Send parts of A to each processor
			tmp = getRowCoordinate(i, sqp) * size;
			for(j = tmp; j < tmp + size; j++) {
				MPI_Send(A[j], M, MPI_INT, i, i, MPI_COMM_WORLD);
			}
		}
	}
	
	for(i = 0; i < size; i++) { //Receive the designated part of A
		MPI_Recv(localA[i], M, MPI_INT, 0, s, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	MPI_Barrier(MPI_COMM_WORLD); //sync
	
	if(s == 0) {
		for(i = 0; i < p; i++) { //Send parts of B to each processor
			tmp = getColCoordinate(i, sqp) * size;
			for(j = 0; j < M; j++) {
				MPI_Send(&(B[j][tmp]), size, MPI_INT, i, i, MPI_COMM_WORLD);
			}
		}
	}
	
	for(i = 0; i < M; i++) { //Receive the designated part of B
		MPI_Recv(localB[i], size, MPI_INT, 0, s, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	MPI_Barrier(MPI_COMM_WORLD); //sync

	multiply(localA, localB, localC, M, size); //multiplay the parts serially
	
	MPI_Barrier(MPI_COMM_WORLD); //sync
	
	
	for(i = 0; i < size; i++) { //Send localC
		MPI_Send(localC[i], size, MPI_INT, 0, i, MPI_COMM_WORLD);
	}
	
	if(s == 0) {
		for(i = 0; i < p; i++) { //Receive parts of C and put them in place
			row = getRowCoordinate(i, sqp) * size;
			col = getColCoordinate(i, sqp) * size;
		
			for(j = 0; j < size; j++) {
				MPI_Recv(&(C[row+j][col]), size, MPI_INT, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD); //sync
	
	t1 = MPI_Wtime();
	
	if(s == 0) { //Display Output
		//printf("\nC:\n");
		//printMatrix(C, M, M);
		printf("\nM: %d\t Processors: %d, time taken: %.8f\n", M, p, (t1-t0));
	}

	
	MPI_Finalize();
	return 0;
}