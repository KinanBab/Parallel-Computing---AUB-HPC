include ${PARALLEL}openmpi.mk
CC=${MPI}
LFLAGS= -lm -lmpi
OBJIP= mpiinprod.o mpiedupack.o
OBJBEN= mpibench.o mpiedupack.o
OBJLU= mpilu_test.o mpilu.o mpiedupack.o
OBJFFT= mpifft_test.o mpifft.o mpiedupack.o
OBJMV= mpimv_test.o mpimv.o mpiedupack.o

all: ip bench lu fft matvec

product: $(OBJIP)
	$(CC)  -o product $(OBJIP) $(LFLAGS)

bench: $(OBJBEN)
	$(CC) -o bench $(OBJBEN) (LFLAGS)

lu: $(OBJLU)
	$(CC) -o lu $(OBJLU) $(LFLAGS)

fft: $(OBJFFT)
	$(CC) -o fft $(OBJFFT) $(LFLAGS)

matvec: $(OBJMV)
	$(CC) -o matvec $(OBJMV) $(LFLAGS)

mpiinprod.o: mpiinprod.c mpiedupack.h
	$(CC) -c mpiinprod.c

mpibench.o: mpibench.c mpiedupack.h
	$(CC) -c mpibench.c

mpilu_test.o: mpilu_test.c mpiedupack.h
	$(CC)  -c mpilu_test.c

mpilu.o: mpilu.c mpiedupack.h
	$(CC) -c mpilu.c

mpifft_test.o: mpifft_test.c mpiedupack.h
	$(CC) -c mpifft_test.c

mpifft.o: mpifft.c mpiedupack.h
	$(CC) -c mpifft.c

mpimv_test.o: mpimv_test.c mpiedupack.h
	$(CC) -c mpimv_test.c

mpimv.o: mpimv.c mpiedupack.h
	$(CC) -c mpimv.c

mpiedupack.o: mpiedupack.c mpiedupack.h
	$(CC) -c mpiedupack.c

clean:
	rm -f *.o ip bench lu fft matvec

runbench: bench
	bsub -oo logs/distributed/sublogs/dlog${log}.log -n ${N} -R"span[ptile=${ptile}]" ${MPI_RUN} ./bench ${MAXH} ${MAXN}

runproduct: product
	bsub -o logs/product_log/shared/sublogs/product${CORES}.log -n ${CORES} -R"span[ptile=${CORES}]" ${MPI_RUN} ./product ${N}
	
