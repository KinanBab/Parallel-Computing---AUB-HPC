#!/bin/bash
for i in 1 2 4 6 8 12 16 
do
	grep Realtime omp${i}.log >> out${i}.log
done

