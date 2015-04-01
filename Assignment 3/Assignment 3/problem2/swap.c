#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Author:	Kinan Dak Al Bab
 * Date:	25/03/2014
 * ID:		201205052
 * Course:	CMPS297M
 */

int n; //vector size
int p; //number of processor

void printArray(int* array, int pid) {
	int i;
	printf("%d: {", pid);
	for(i = 0; i < n; i++)
		printf("%d, ", array[i]);
	printf("}\n");
}

int main(int argc, char * argv[]) {	
	MPI_Init(&argc, &argv);

	int s, i;

	sscanf(argv[1], "%d", &n); //getting vector size

    MPI_Comm_rank(MPI_COMM_WORLD, &s); //getting processor ID
    MPI_Comm_size(MPI_COMM_WORLD, &p); //getting processor SIZE, Assumed to be at least 2.

	srand(time(NULL) + s);

	int* a = (int*) malloc(sizeof(int) * n);
	int* b = (int*) malloc(sizeof(int) * n);

	for(i = 0;i < n;i++) {
		a[i] = rand() % 1000;
	}

	printArray(a, s); //print the As

	MPI_Barrier(MPI_COMM_WORLD); //sync

	if(s == 0) {
		MPI_Send(a, n, MPI_INT, 1, 0, MPI_COMM_WORLD); //Send A0 to B1
		MPI_Recv(b, n, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //Recieve A1 in B0
	} else if(s == 1) {
		MPI_Recv(b, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //Recive A0 in B1
		MPI_Send(a, n, MPI_INT, 0, 0, MPI_COMM_WORLD); //Send A1 toB0
	}

	MPI_Barrier(MPI_COMM_WORLD); //Sync

	printArray(b, s); //print the Bs, assert that B0 = A1 and B1 = A0

	MPI_Finalize();	//end

	return 0;
}
