include ${PARALLEL}openmpi.mk
N=2
SIZE=500

all: swap.o
	bsub -oo swap.log -n ${N} -R"span[ptile=1]" ${MPI_RUN} ./swap.o ${SIZE}

swap.o: swap.c
	${MPI} swap.c -o swap.o

.PHONY: clean
clean:
	-rm *.o *.log
