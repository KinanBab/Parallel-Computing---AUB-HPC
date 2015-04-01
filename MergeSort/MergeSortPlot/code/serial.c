#include "common.c"

int main(int argc, char *argv[])
{
	int threads;
	int n;
	merge_init(argc, argv, &threads, &n);
     
	printf("[Threads] %d\n[N] %d\n", threads, n); 
	//int a[n], t[n];
	int * a = (int *) malloc(n * sizeof(int));
	int * t = (int *) malloc(n * sizeof(int));
	randomize_array(a, n, 1, n * 5);
	mergesort_serial(a, n, t);
	//if(!verify_sorted(a, n))
	//	printf("Failed\n");
	free(a);
	free(t);
	return(0);
}
