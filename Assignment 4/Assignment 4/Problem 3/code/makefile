include ${PARALLEL}openmpi.mk
include ${PARALLEL}cilk.mk
# Input Size
#INPUT=700000000
#INPUT=100000000
#Inclusion for <bench/*> and <stack/*>
FLAGS=-Icommon
# Number of MPI nodes to use for strictly distributed mode
#NODES=4
# Number of MPI nodes to use for Hybrid
#H_NODES=4
# Number of shared processes to use
#THREADS=8

# Compile All
all: shared mpi hybrid cilk
# Shared Mergesort
shared: shared.c common.c
	gcc ${FLAGS} shared.c -o shared -fopenmp	
# Distributed Mergesort
mpi: mpi.c common.c
	${MPI} ${FLAGS} -g -lm mpi.c -o mpi
# Hybrid Mergesort
hybrid: hybrid.c common.c
	${MPI} ${FLAGS} -fopenmp -g -lm hybrid.c -o hybrid
# Compile the CILK shared mode
cilk: common.c shared.cilk
	${CILK} ${CILK_PROFILE} ${FLAGS} ${CILK_FLAGS} shared.cilk -o sharedcilk
# Run
runomp:
	bsub -o logs/omp-shared/omp${THREADS}.log -n ${THREADS} -R"span[ptile=8]" ./shared ${THREADS} ${INPUT}
runcilk:
	bsub -o logs/cilk-shared/cilk${THREADS}.log -n ${THREADS} -R"span[ptile=16]" ./sharedcilk --stats 1 --nproc ${THREADS} ${INPUT}
runhybrid:
	bsub -o logs/hybrid/hybrid${THREADS}_${H_NODES}.log -n ${H_NODES} -R"span[ptile=1]" ${MPI_RUN} ./hybrid ${THREADS} ${INPUT}
runmpi:	
	bsub -o logs/mpi-distributed/mpi${NODES}.log -n ${NODES} -R"span[ptile=1]"  ${MPI_RUN} ./mpi ${INPUT}
clean:
	rm *.log
