/**************************************************************************/
/************************* Name: Kinan Dak Al Bab *************************/
/************************* Course: CMPS297M       *************************/
/**************************************************************************/

/* OpenMP code to compute Fibonacci */
#include <stdlib.h>
#include <stdio.h>
#include "omp.h"
int baseCase;

static int fib(int);
static int sequentailFib(int);

int main(int argc, char* argv[]) {
	int n = atoi(argv[1]);
	int nthreads = atoi(argv[2]);
	baseCase = atoi(argv[3]);

	double time0, time1;
	unsigned long result;
	
	time0 = omp_get_wtime();
	#pragma omp parallel num_threads(nthreads) 
		{
		#pragma omp single
			{
			result = fib(n);
			}
		} // all threads join master thread and terminates
	time1 = omp_get_wtime();
 
	printf("%f,%d,%d,%d,kinan\nFibonacci Number %d is: %d", time1-time0, n, nthreads, baseCase, n, result);
}

static int fib(int n){
	unsigned long i, j;
	if(n < baseCase) 
		return sequentailFib(n);
		
	if(n < 2)
		return n;
		
	#pragma omp task shared (i)
		{
		i = fib(n-1);
		}
	
	#pragma omp task shared (j)
		{
		j = fib(n-2);
		}
	
	#pragma omp taskwait
	
	return (i+j);
}

static int sequentailFib(int n) {
	if (n < 2) {
		return n;
	}

	return sequentailFib(n - 1) + sequentailFib(n - 2);
}
