include ${PARALLEL}openmpi.mk

pi-reduction: pi-reduction.c
	gcc ${FLAGS} pi-reduction.c -o pi-reduction.o -fopenmp
	
pi-critical: pi-critical.c
	gcc ${FLAGS} pi-critical.c -o pi-critical.o -fopenmp
	
pi-array: pi-array.c
	gcc ${FLAGS} pi-array.c -o pi-array.o -fopenmp
	
run-reduction: pi-reduction
	bsub -o logs/runs/reduction.log -n ${THREADS} -R"span[ptile=${THREADS}]" ./pi-reduction.o ${STEPS} ${THREADS}
	
run-critical: pi-critical
	bsub -o logs/runs/critical.log -n ${THREADS} -R"span[ptile=${THREADS}]" ./pi-critical.o ${STEPS} ${THREADS}
	
run-array: pi-array
	bsub -o logs/runs/array.log -n ${THREADS} -R"span[ptile=${THREADS}]" ./pi-array.o ${STEPS} ${THREADS}

.PHONY: clean
clean:
	rm *.o
