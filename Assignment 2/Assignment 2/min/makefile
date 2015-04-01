include ${PARALLEL}openmpi.mk
N=8

run-min: min
	bsub -oo runs/shared/log${SIZE}.log -n ${N} -R"span[ptile=${N}]" ${MPI_RUN} ./min ${SIZE}

min: min-mpi.c
	${MPI} -o min min-mpi.c

.PHONY: clean
clean:
	-@rm min
	-@rm runs/*summary*
	-@rm runs/shared/*.log
	-@rm runs/distributed/*.log

