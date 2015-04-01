/**************************************************************************/
/************************* Name: Kinan Dak Al Bab *************************/
/************************* Course: CMPS297M       *************************/
/**************************************************************************/
#include <omp.h>
#include <stdio.h>

int num_steps;
double step; 

void main (int argc, char* argv[]) {
	num_steps = atoi(argv[1]);
	int threads = atoi(argv[2]);
	
	int i;
	double time0, time1, x, tmp, pi, sum = 0.0; 
	step = 1.0 / (double) num_steps; 
	
	time0 = omp_get_wtime();
	
	#pragma omp parallel for num_threads(threads) private(x, tmp)
		for (i=0; i < num_steps; i++) {
			x = (i + 0.5) * step;
			tmp = 4.0 / (1.0 + x * x);
			
			#pragma omp critical
			sum = sum + tmp;
		}
		
	pi = step * sum; 
	
	time1 = omp_get_wtime();
	printf("%f,%d,%d,kinan\n", time1-time0, num_steps, threads);
}