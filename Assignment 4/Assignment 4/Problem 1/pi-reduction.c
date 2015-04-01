/**************************************************************************/
/************************* Name: Kinan Dak Al Bab *************************/
/************************* Course: CMPS297M       *************************/
/**************************************************************************/
#include <omp.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	int num_steps = atoi(argv[0]);
	int threads = atoi(argv[2]);
	double step;
	
	int i;
	double time0, time1, x, pi, sum = 0.0; 
	step = 1.0 / (double) num_steps; 
	
	time0 = omp_get_wtime();
	
	#pragma omp parallel for num_threads(threads) private(x) reduction(+:sum)
		for (i=0; i < num_steps; i++) {
			x = (i + 0.5) * step;
			sum = sum + 4.0 / (1.0 + x * x);
		}
		
	pi = step * sum; 
	
	time1 = omp_get_wtime();
	printf("%f,%d,%d,kinan\n", time1-time0, num_steps, threads);
}