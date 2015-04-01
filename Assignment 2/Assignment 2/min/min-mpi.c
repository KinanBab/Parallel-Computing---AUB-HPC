#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

/*
 * Author:	Kinan Dak Al Bab
 * Date:	13/03/2014
 * ID:		201205052
 * Course:	CMPS297M
 * Assignment 2
 */

int startPosition(int s, int p, int n) {
	return s * ceil((double)n/p);
}

int count(int start, int p, int n) {
	return MIN(start + ceil((double)n/p), n) - start;
}

int main(int argc, char **argv){
	int n, p, s, i, start, n1, min, local_min, index, local_index, tmp;
	double time, time0, time1;

	MPI_Init(&argc,&argv);
	
    MPI_Comm_size(MPI_COMM_WORLD,&p); //Number of processors
    MPI_Comm_rank(MPI_COMM_WORLD,&s); //Processor number

	sscanf(argv[1], "%d", &n); //Read length from command line
    MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);

	
	int* array;
	if(s == 0) { //Generate The Array
		array = (int*) malloc(sizeof(int) * n);
		srand(MPI_Wtime());
		
		for(i = 0;i < n;i++) {
			array[i] = rand() % 10000;
		}
	}
	
	MPI_Barrier(MPI_COMM_WORLD); //Sync
    time0= MPI_Wtime(); //Initialize timer

	int* subarray = (int*) malloc(sizeof(int) * n1);
	
	if(s == 0) { //Processor 0 send to each processor its block
		for(i = 0; i < p; i++) {
			start = startPosition(i, p, n);
			n1 = count(start, p, n);
			
			MPI_Send(&(array[start]), n1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}

	start = startPosition(s, p, n); //each processor receives its block
	n1 = count(start, p, n);
	MPI_Recv(subarray, n1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    MPI_Barrier(MPI_COMM_WORLD); //sync

	local_min = subarray[0]; //find local min
	for(i = 1; i < n1; i++) {
		if(subarray[i] < local_min) {
			local_min = subarray[i];
			local_index = i + start;
		}
	}
	
    MPI_Barrier(MPI_COMM_WORLD); //sync
	
	MPI_Allreduce(&local_min, &min, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD); //find global Min
	
	if(s == 0) { //Send the minimum to all processor and let the processor with the minimum send the index
		for(i = 0; i < p; i++) {			
			MPI_Send(&min, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
 
	MPI_Recv(&tmp, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //recieve the minimum

	if(local_min != tmp) { //if the processor was not the one who found min, take out its local index (only keep the local index of the minimum)
		local_index = -1;
	}
	
	MPI_Allreduce(&local_index, &index, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD); //find the greatest index of min
	
	time1= MPI_Wtime(); //Calculate the time
	time = time1 - time0;

	free(subarray);
	
	if(s == 0) { //output Result
		printf("ARRAY : {");
		for(i = 0; i < n-1; i++) {
			printf("%d, ", array[i]);
		}
		printf("%d}\n", array[n-1]);
		
		printf("Min : %d \t\t Index: %d\n", min, index);
		printf("Number of Processors: %d \t Input Size: %d \t Time Taken : %.8f\n", p, n, time);
		
		free(array);
	}

	MPI_Finalize();
	
	exit(0);
}
