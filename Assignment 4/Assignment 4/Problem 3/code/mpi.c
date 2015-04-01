/*
 * Based on: 
 * Radenski A., 
 * Shared Memory, Message Passing, and Hybrid Merge Sorts for Standalone and Clustered SMPs.
 * Proceedings PDPTA'11, the 2011 International Conference on Parallel and Distributed Processing Techniques and Applications,
 * CSREA Press,
 * 2011.
 * http://www1.chapman.edu/~radenski/research/papers/mergesort-pdpta11.pdf
 */
#include <mpi.h>
#include <math.h>
#include "common.c"


int threads;
int my_topmost_level_mpi(int rank)
{
	int level = 0;
	while(pow(2, level) <= rank) level++;
	return level;
}

/*
 * Mergesort by sending the right subtree and recursing on left subtree
 * 1. Send [right] subtree to a helper thread to sort it
 * 2. Sort [left] subtree recursively
 * 3. Receive sorted [right] subtree from helper (helper sends it in run_helper_mpi)
 * 4. Merge [left, right]
 * see fig.3 page 3 for illustration
 * Args:
 * <a> 		array to sort
 * <size> 	size of array to sort
 * <temp>	temporary array (same size as a)
 * <level>	recursion level, how deep in the tree are we?
 * <rank>	mpi process id (to not recall MPI_Comm_rank on every call)
 */
void mergesort_parallel_mpi(int a[], int size, int temp[], int level, int rank)
{
	//Who should receive my right tree?
	int helper_rank = rank + pow(2, level);
	//Exhausted all possble threads?
	if(helper_rank > (threads-1)){
		mergesort_serial(a, size, temp);
	}
	else
	{
		//Dummies
		MPI_Request r;
		MPI_Status status;
		//Send Second half of array asynchornously to helper
		MPI_Isend(a + size/2, size-size/2, MPI_INT, helper_rank,
				0, MPI_COMM_WORLD, &r);
		mergesort_parallel_mpi(a, size/2, temp, level+1, rank);
		//[!] Blocking
		//Wait until right subtree is done
		MPI_Recv(a + size/2, size-size/2, MPI_INT, helper_rank, 0,
				MPI_COMM_WORLD, &status);
		//Once done merge
		merge(a, size, temp);
	}
}
//Master thread program
void run_root_mpi(int a[], int size, int temp[])
{
	//Level = 0 Rank = 0
	mergesort_parallel_mpi(a, size, temp, 0, 0);
}
//Helper thread program
void run_helper_mpi(int rank)
{
	MPI_Status status;
	//[!] Blocking	
	//Wait to receive some data from a random MPI thread
	MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
   	//Figure out the sender
 	 int sender = status.MPI_SOURCE;
   	int n;
   	//Figure out the size
   	MPI_Get_count(&status, MPI_INT, &n);
   	//Allocate approriately
   	int * a = (int *) malloc(sizeof(int) * n);
   	int * t = (int *) malloc(sizeof(int) * n);
   	//[!] Blocking
   	//Receive the data
   	 MPI_Recv(a, n, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
 	//Determine depth in the tree based on rank
   	int level = my_topmost_level_mpi(rank);
   	//Sort the data I received
  	 mergesort_parallel_mpi(a, n, t, level, rank);
   	//Send it back to whoever gave it to me
   	MPI_Send(a, n, MPI_INT, sender, 0, MPI_COMM_WORLD);
   	//Cleanup
   	free(a);
   	free(t);
}
int main(int argc, char *argv[])
{
	int namelen;
	char cpuname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);
	long n;
	merge_init_mpi(argc,argv, &n);
	
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &threads);
	MPI_Get_processor_name(cpuname, &namelen);

	//Helper
	if(rank != 0)
	{
		//Run helper routine on helper rank
		run_helper_mpi(rank);
		MPI_Finalize();
		exit(0);
	}
	//Master thread
	int * a = (int *) malloc(n * sizeof(int));
	int * t = (int *) malloc(n * sizeof(int));
	randomize_array(a, n, 1, n * 5);
	printf("[Nodes] %d\n", threads);

	bench_mark(&bclock);
	bench_mark(&breal);
	
	//Run the master MPI
	run_root_mpi(a, n, t);

	bench_unmark_all();

	//if(!verify_sorted(a, n))
	//	exit(1);
	free(a);
	free(t);
	MPI_Finalize();
        return(0);
}
