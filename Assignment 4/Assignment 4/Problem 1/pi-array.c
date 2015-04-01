/**************************************************************************/
/************************* Name: Kinan Dak Al Bab *************************/
/************************* Course: CMPS297M       *************************/
/**************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int num_steps;
double step; 

void main (int argc, char* argv[]) {
	num_steps = atoi(argv[1]);
	int threads = atoi(argv[2]);
	
	int i;
	double time0, time1, x, pi, sum = 0.0; 
	step = 1.0 / (double) num_steps; 
	
	time0 = omp_get_wtime();
	
	double* sums = (double*) malloc (sizeof(double) * threads);
	for(i = 0; i < threads;i++) {
		sums[i] = 0;
	}
	
	#pragma omp parallel for num_threads(threads) private (x)
		for (i=0; i < num_steps; i++) {
			int tid = omp_get_thread_num();
			
			x = (i + 0.5) * step;
			sums[tid] = 4.0 / (1.0 + x * x);
		}
	
	for(i = 0; i < threads; i++) {
		pi += sums[i] * step;
	}
	
	time1 = omp_get_wtime();
	printf("%f,%d,%d,kinan\n", time1-time0, num_steps, threads);
}