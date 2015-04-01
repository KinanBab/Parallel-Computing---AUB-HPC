#include <mcbsp.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Author:	Kinan Dak Al Bab
 * Date:	25/03/2014
 * ID:		201205052
 * Course:	CMPS297M
 */

int p; //Number of processors
int N, M; //Number of row, cols

void twoDarray() {
	int s, i, j;
	int** array;

	bsp_begin(p); //Begin Parallel

	s = bsp_pid();

	array = (int**) malloc(sizeof(int*) * N);
	for(i = 0;i < N;i++) {
		array[i] = (int*) malloc(sizeof(int) * M);
	}
	
	for(i = 0;i < N;i++) {
		bsp_push_reg(&(array[i]), sizeof(int) * M); //pushing each row alone
	}

    bsp_sync();

	if(s == 0) {
		for(i = 0; i < p; i++) {
			for(j = 0; j < N; j++) {
				bsp_put(i, &(array[j]), &(array[j]), 0, sizeof(int) * M);
			}
		}
	}

    bsp_sync();

	//Now each processor has the array
	
    bsp_end();
}


int main(int argc, char **argv) {
	bsp_init(twoDarray, argc, argv);
	
	sscanf(argv[1], "%d", &p); //Getting number of processors
	sscanf(argv[2], "%d", &N); //Getting number of rows
	sscanf(argv[3], "%d", &M); //Getting number of cols

	twoDarray();	

    exit(0);
}
