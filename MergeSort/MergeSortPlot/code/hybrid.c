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
#include <mpi.h>
#include <math.h>
#include "common.c"

int threads, nodes;
int my_topmost_level_mpi(int rank)
{
	int level = 0;
	while(pow(2, level) <= rank) level++;
	return level;
}

//Shared mergesort: see shared.c
void mergesort_shared(int a[], int size, int temp[], int threads)
{
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
                                temp + size/2 , threads - threads/2);


                }
                merge(a, size, temp);
        }
}


//mpi mergesort see mpi.c, modified base case
void mergesort_parallel_mpi(int a[], int size, int temp[], int level, int rank)
{
	int helper_rank = rank + pow(2, level);
	//Are we deep enough in the tree?
	if(helper_rank > (nodes-1)){
		//Use shared to resolve base case
		mergesort_shared(a, size, temp, threads);
	}
	else
	{
		MPI_Request r;
		MPI_Status status;
		//Send Second half of array asynchornously
		//i.e right branch of the tree
		MPI_Isend(a + size/2, size-size/2, MPI_INT, helper_rank,
				0, MPI_COMM_WORLD, &r);
		//Recurse on the left branch of the tree
		mergesort_parallel_mpi(a, size/2, temp, level+1, rank);
		//Block and wait
		MPI_Recv(a + size/2, size-size/2, MPI_INT, helper_rank, 0,
				MPI_COMM_WORLD, &status);
		merge(a, size, temp);
	}
}

void run_root_mpi(int a[], int size, int temp[])
{
	//Level = 0 Rank = 0
	mergesort_parallel_mpi(a, size, temp, 0, 0);
}
void run_helper_mpi(int rank)
{
    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    int sender = status.MPI_SOURCE;
    int n;
    MPI_Get_count(&status, MPI_INT, &n);
    int * a = (int *) malloc(sizeof(int) * n);
    int * t = (int *) malloc(sizeof(int) * n);
    MPI_Recv(a, n, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    int level = my_topmost_level_mpi(rank);
    mergesort_parallel_mpi(a, n, t, level, rank);
    MPI_Send(a, n, MPI_INT, sender, 0, MPI_COMM_WORLD);
    free(a);
    free(t);
}
int main(int argc, char *argv[])
{
	int provided;
	int namelen;
	char cpuname[MPI_MAX_PROCESSOR_NAME];
	//All our threads will use MPI unrestricted
	//Overlapped mode
        MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	long n;
	merge_init(argc,argv, &threads, &n);
	//Set OMP options
	omp_set_dynamic(0);
	omp_set_nested(1);
	omp_set_num_threads(threads);	
	//Get MPI information
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nodes);
	MPI_Get_processor_name(cpuname, &namelen);

	//Helpers
	if(rank != 0)
	{
		run_helper_mpi(rank);
		MPI_Finalize();
		exit(0);
	}
	//Master
	int * a = (int *) malloc(n * sizeof(int));
	int * t = (int *) malloc(n * sizeof(int));
	randomize_array(a, n, 1, n * 5);
	printf("[Threads] Nodes: %d Threads/Node: %d\n", nodes, threads);

	bench_mark(&bclock);
	bench_mark(&breal);
	
	run_root_mpi(a, n, t);

	bench_unmark_all();

	//if(!verify_sorted(a, n))
	//	exit(1);
	free(a);
	free(t);
	MPI_Finalize();
        return(0);
}
