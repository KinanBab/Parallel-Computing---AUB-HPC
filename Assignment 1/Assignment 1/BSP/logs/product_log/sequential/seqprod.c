/*
 * Author:	Kinan Dak Al Bab.
 * Date:	21/02/2014.
 * ID:		201205052
 * Course:	CMPS297M
 * Sequential code for computing the sum of integral squares between 
 * 1 and n, Problem 2 of Assignment 1. 
 */

#include <stdio.h>
#include <stdlib.h>

int n; //The upper bound for the numbers to be squared and summed.

int main(int argc, char** argv) {
	sscanf(argv[1], "%d", &n);

	int sum = 0;

	int i = 0;
	for(;i < n;i++) {
		sum+= i*i;
	}

	printf("%d", sum);
}
