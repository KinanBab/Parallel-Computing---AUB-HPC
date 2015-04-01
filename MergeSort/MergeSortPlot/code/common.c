#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"insertion.c"
#include<bench/clock.h>
#include<bench/real.h>
#ifndef SMALL
#define SMALL 500
#endif

//Copy size amount of elements from b into a 
void copy_array(int a[], int b[], int size);
//Print array from 0 to size-1
void print_array(int a[], int size);
//Fill an array with random data between min-max
void randomize_array(int a[], int size, int min, int max);
//Check if array is sorted theta(size)
short int verify_sorted(int a[], int size);
//Initialize for distributed MPI (n = input size) 
void merge_init_mpi(int argc, char ** argv, long * n);
//Initialize for shared computation (threads and n = input size
void merge_init(int, char **, int * threads, long * n);
//Merge the 2 lists of length: size/2: 
//List 1: [0, size/2] 
//List 2: [size/2 + 1, size - 1]
//Using b as temporary
void merge(int a[], int size,  int b[]);
//Serial Mergesort on a using temp array temp of size size
void mergesort_serial(int a[], const int size, int temp[]);

//Define benchmark formats/measures
//Dump to stdout
BenchMeasure * bclock() { return measure_clocktime(stdout, "[Clocktime]\t"); }
BenchMeasure * breal () { return measure_realtime (stdout, "[Realtime]\t" ); }


void print_array(int a[], int size)
{
	int i;
	for(i = 0; i < size; i++)
		printf("%12d -> %12d\n", i, a[i]);
}

void randomize_array(int a[], int size, int min, int max)
{
	int i;
	srand(time(NULL));
	for(i = 0; i < size; i++)
		a[i] = (rand() % (max - min)) + min + 1;
}

void copy_array(int a[], int b[], int size)
{
	int  i;
	for(i = 0; i < size; i ++) a[i] = b[i];	
}

short int verify_sorted(int a[], int size)
{
	int i;
	for(i = 0; i < size -1; i++)
	{
		if(a[i] > a[i + 1])
			return 0;
	}
	return 1;
}
void merge_init_mpi(int argc, char ** argv, long * n)
{
	if(argc != 2)
	{
		printf("Args: <n>\n");
		exit(1);
	}
	*n = atoi(argv[1]);
}
void merge_init(int argc, char ** argv, int * threads, long * n)
{
	if(argc != 3) 
	{
		printf("Args: <threads> <n>\n");
		exit(1);
	}
	*threads = atoi(argv[1]);
	*n	 = atoi(argv[2]);
	
}

void merge(int a[], int size,  int b[])
{
	int i 	= 0;
	int i1 	= 0;

	int i2 	= (size / 2);
	
	int e1	= i2 - 1;
	int e2	= size - 1;

	for(i = 0; i < size; i++)
	{
		if(i1 > e1)
		{
			b[i] = a[i2];
			i2++;
		}
		else if (i2 > e2)
		{
			b[i] = a[i1];
			i1++;
		}
		else
		{
			if(a[i1] <= a[i2])
			{
				b[i] = a[i1];
				i1++;
			}
			else
			{
				b[i] = a[i2];
				i2++;
			}
		}
	}
	copy_array(a, b, size);
}
void mergesort_serial(int a[], const int size, int temp[])
{
	if(size < SMALL)
	{
		//Use insertion_sort as base case
		insertion_sort(a, size);
		return;
	}
	mergesort_serial(a, size/2, temp);
	mergesort_serial(a + size/2, size - size/2, temp);
	merge(a, size, temp); 		
}
