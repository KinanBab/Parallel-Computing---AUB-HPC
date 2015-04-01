#!/bin/bash
for N in 1 2 4 6 8
do
	for INPUT in 10000 1000000 100000000 200000000 400000000 500000000
	do
		make runmpi NODES=${N} INPUT=${INPUT}
	done
done
