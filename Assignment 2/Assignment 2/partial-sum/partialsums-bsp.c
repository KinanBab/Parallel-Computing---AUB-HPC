#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mcbsp.h>
#include <time.h>
#include <math.h>

#define MAX(a,b) ((a)>(b) ? (a) : (b))
#define MIN(a,b) ((a)<(b) ? (a) : (b))

/*
 * Author:	Kinan Dak Al Bab
 * Date:	13/03/2014
 * ID:		201205052
 * Course:	CMPS297M
 */

int p; //number of processors
int n; //size of input
int k; //size of input after padding if original size is not a power of 2

int* array; //array to partial sum

int blocksize() {
	return ceil((double)n/p);
}

void partialsums() {
	int s, d, i, tmp, n1;	
	double time, time0, time1;
	
	bsp_begin(p); //Begin Parallel
	
    s = bsp_pid();
	n1 = blocksize();
	
	time0 = bsp_time();
	
	tmp = 1;
	for(d = 0; tmp < n; d++) { //first pass
		for(i = s * (tmp*2); i < n; i+= p * (tmp*2)) {
			if((i + (tmp*2) - 1) < n) {
				array[i + (tmp*2) - 1] = array[i + tmp - 1] + array[i + (tmp*2) - 1];
			}
		}
		tmp = tmp * 2;
		
		bsp_sync();  
	}
	
	if(s == 0) { //set last element to zero
		array[n-1] = 0;
	}
	
	tmp = tmp / 2;
	for(d = d-1; d >= 0; d--) { //second pass
		for(i = s * (tmp*2); i < n; i+= p * (tmp*2)) {
			if((i + (tmp*2) - 1) < n) {
				int temp = array[i + tmp - 1];
				array[i + tmp - 1] = array[i + (tmp*2) - 1];
				array[i + (tmp*2) - 1] = temp + array[i + (tmp*2) - 1];
			}
		}
		tmp = tmp / 2;	
		
		bsp_sync();  
	}
	
	time1 = bsp_time();
	time = time1 - time0;
	
	if(s == 0) { //printResult 
		printf("Number of Processors: %d \t Input Size: %d \t Time Taken: %.8f", p, k, time);
	}
	
    bsp_end();
}

int main(int argc, char **argv){
    bsp_init(partialsums, argc, argv);

	int i; 
	srand(time(NULL));
	
	sscanf(argv[1], "%d", &p);
	sscanf(argv[2], "%d", &n);
	
	k = (int) pow(2, ceil((double)log10(n) / log10(2))); //if n is not a power of 2 find the closest greater power of 2
	
	array = (int*) malloc(sizeof(int) * k);
	
	for(i = 0;i < n;i++)  {
		array[i] = rand() % 5;
	}
	
	for(i = n;i < k;i++) { //fill the rest of the array (between n and the closest greater power of 2) with 0
		array[i] = 0;

	}
	
	int tmp = n;
	n = k;
	k = tmp;
	
    if (p > bsp_nprocs()){
        printf("Sorry, not enough processors available.\n"); fflush(stdout);
        exit(1);
    }

    partialsums();

    exit(0);
}
