/*
 * Based on: 
 * Radenski A., 
 * Shared Memory, Message Passing, and Hybrid Merge Sorts for Standalone and Clustered SMPs.
 * Proceedings PDPTA'11, the 2011 International Conference on Parallel and Distributed Processing Techniques and Applications,
 * CSREA Press,
 * 2011.
 * http://www1.chapman.edu/~radenski/research/papers/mergesort-pdpta11.pdf
 */

#include <omp.h>
#include "common.c"
/*
 * Shared mergesort for OpenMP
 * threads is used as control to reach the base case
 * keep halfing usable threads to reach base case
 * thereby distributing the tree
 * Args:
 * <a>		Array to sort
 * <size>	Size of array
 * <temp>	Temporary array
 * <threads>	Number of threads to use
 */
void mergesort_shared(int a[], int size, int temp[], int threads)
{
	//Alternatively:
	//if(size < SMALL)
	if(threads == 1)
	{
		mergesort_serial(a, size, temp);
	}
	else
	{
		#pragma omp parallel sections
		{
			#pragma omp section
			mergesort_shared(a, size/2, temp, threads/2);
			#pragma omp section
			mergesort_shared(a + size/2, size - size/2,
   				temp + size/2, threads - threads/2);
       

		}
		merge(a, size, temp);
	}
}

int main(int argc, char *argv[])
{
	int threads;
	long n;
	merge_init(argc, argv, &threads, &n);
	omp_set_dynamic(1);
	omp_set_nested(1);
	omp_set_num_threads(threads);
 
	printf("[Threads] %d\nInput %d\n", threads, n); 
	int * a = (int *) malloc(n * sizeof(int));
	int * t = (int *) malloc(n * sizeof(int));
	randomize_array(a, n, 1, n * 5);

	bench_mark(&bclock);
	bench_mark(&breal);
	mergesort_shared(a, n, t, threads);
	bench_unmark_all();

	//printf("[Verifying]\n");	
	//if(!verify_sorted(a, n))
	//	printf("Failed\n");
	free(a);
	free(t);
	return(0);
}
