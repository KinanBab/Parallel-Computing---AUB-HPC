#include <mcbsp.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))
/*
 * Author:	Kinan Dak Al Bab
 * Date:	13/03/2014
 * ID:		201205052
 * Course:	CMPS297M
 * Count sort, paralleliazing C version (C array of counts)
 */

int p;
int n;

int parallelMin(int* A, int s) { 
	int blocksize, localMin, i, limit;
	int* mins;
	
	blocksize = ceil((double)n/p); //get block size
	
	i = blocksize * s;
	limit = MIN(n, blocksize * (s+1));
	
	localMin = A[i];
	for(i += 1; i < limit;i++) {
		localMin = MIN(localMin, A[i]);
	}
	
	mins = (int*) malloc(sizeof(int) * p);
	bsp_push_reg(mins, sizeof(int) * p); //Make mins visible globally
	bsp_sync(); //sync
	
	bsp_put(0, &localMin, mins, s * sizeof(int), sizeof(int)); //send localMax to P0
	bsp_sync();
	
	if(s == 0) {
		localMin = mins[0];
		for(i = 1; i < p; i++) {
			localMin = MIN(localMin, mins[i]);
		}
	}
	
	bsp_push_reg(&localMin, sizeof(int)); //Make localMin visible globally
	bsp_sync();
	
	bsp_get(0, &localMin, 0, &localMin, sizeof(int)); //each processor gets the min
	bsp_sync();
	
	return localMin;
}

int parallelMax(int* A, int s) {
	int blocksize, localMax, i, limit;
	int* maxs;	
	blocksize = ceil((double)n/p); //get block size
	
	i = blocksize * s;
	limit = MIN(n, blocksize * (s+1));
	
	localMax = A[i];
	for(i += 1; i < limit;i++) {
		localMax = MAX(localMax, A[i]);
	}
	
	maxs = (int*) malloc(sizeof(int) * p);
	bsp_push_reg(maxs, sizeof(int) * p); //Make maxs visible globally
	bsp_sync(); //sync
	
	bsp_put(0, &localMax, maxs, s * sizeof(int), sizeof(int)); //send localMax to P0
	bsp_sync();
	
	if(s == 0) {
		localMax = maxs[0];
		for(i = 1; i < p; i++) {
			localMax = MAX(localMax, maxs[i]);
		}
	}
	
	bsp_push_reg(&localMax, sizeof(int)); //Make localMax visible globally
	bsp_sync();
	
	bsp_get(0, &localMax, 0, &localMax, sizeof(int)); //each processor gets the min
	bsp_sync();
	
	return localMax;
}

void countSort() {
	int s, i, j, min, max, blocksize, start, limit, localCount, index;
	double time, time0, time1;
	int *A, *C, *localB, *sizes;
	
	bsp_begin(p); //Begin Parallel
	
    s = bsp_pid(); //Current Processor Number
	
	A = (int*)malloc(sizeof(int) * n);
	
	if(s == 0) {
		for(i = 0; i < n; i++) { //Generating the array
			A[i] = rand() % 100;
		}
	}
	
	bsp_sync(); //sync
	
	time0 = bsp_time();
	
	bsp_push_reg(A, sizeof(int) * n); //push the array
	bsp_sync(); //sync

    bsp_get(0, A, 0, A, sizeof(int)* n); //Get the array
	bsp_sync(); //sync
	
	min = parallelMin(A, s); //find min of A
	max = parallelMax(A, s); //find max of A
	
	blocksize = ceil((double)(max - min + 1)/p); //get block size
	
	start = blocksize * s; //start index in C
	limit = MIN(max - min + 1, blocksize * (s+1)); //end index in C
	
	C = (int*) malloc(sizeof(int) * blocksize); //init C to 0
	for(i = 0; i < blocksize; i++) {
		C[i] = 0;
	}
	
	localCount = 0;
	for(i = 0; i < n;i++) { //fill C for values in Range[start, limit[
		int tmp = A[i] - min;
		if(start <= tmp && tmp < limit) {
			C[tmp - start] += 1;
			localCount++;
		}
	}
	
	if(localCount > 0) {
		localB = (int*)malloc(sizeof(int) * localCount);
		
		int tmp = limit - start;
		int j = 0;
		for(i = 0; i < tmp; i++) { //Generate localB from C
			if(C[i] > 0) {
				localB[j] = i + start + min;
				C[i] = C[i] - 1;
				i--;
				j++;
			}
		}
	}
	
	sizes = (int*) malloc(sizeof(int) * p);
	bsp_push_reg(sizes, sizeof(int) * p);
	bsp_sync(); //sync
	
	bsp_put(0, &localCount, sizes, s * sizeof(int), sizeof(int)); //send localCount to P0
	bsp_sync(); //sync
	
	index = 0;
	bsp_push_reg(&index, sizeof(int));
	bsp_sync(); //sync
	
	if(s == 0) { //Processor 0 sends start index to all processors	
		int tmp = 0;
		for(i = 0; i < p; i++) {
			bsp_put(i, &tmp, &index, 0, sizeof(int));
			tmp += sizes[i];
		}
	}
	
	bsp_sync(); //sync
	
	bsp_put(0, localB, A, index * sizeof(int), sizeof(int) * localCount); //put localB in its place in A
	bsp_sync(); //A now contains the final sorted array
	
	time1 = bsp_time();
	time = time1 - time0;
	
	if(s == 0) { //printing Result
		printf("Number of processors: %d \t Input Size: %d \t Time Taken: %.8f", p, n, time);
	}
	
	bsp_end();
}

int main(int argc, char** argv) {
	bsp_init(countSort, argc, argv);
	
	srand(time(NULL));
	
	sscanf(argv[1],"%d",&p);
	sscanf(argv[2],"%d",&n);

	countSort();
	
	exit(0);
}
