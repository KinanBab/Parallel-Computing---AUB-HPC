
#!bin/bash

for i in 1 2 4 6 8 
do
	grep Realtime mpi${i}.log >> time${i}.log
done
