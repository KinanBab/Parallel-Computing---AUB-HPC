include ${PARALLEL}openmpi.mk
N=1

all: multi.o
	bsub -oo logs/matrix${M}.log -n ${N} -R"span[ptile=1]" ${MPI_RUN} ./multi.o ${M}

multi.o: multi.c
	${MPI} multi.c -o multi.o

.PHONY: clean
clean:
	-rm *.o 
