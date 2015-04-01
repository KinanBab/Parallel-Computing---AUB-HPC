include ${PARALLEL}openmpi.mk

run: fibonacci.o
	bsub -o logs/runs/fibonacci${BASECASE}.log -n ${THREADS} -R"span[ptile=${THREADS}]" ./fibonacci.o ${N} ${THREADS} ${BASECASE}

fibonacci.o: fibonacci.c
	gcc ${FLAGS} fibonacci.c -o fibonacci.o -fopenmp

.PHONY: clean
clean:
	rm *.o
