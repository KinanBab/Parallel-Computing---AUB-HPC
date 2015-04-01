include ${PARALLEL}mbsp.mk
N=4
rows=1000
cols=5000

all: 1Darray.o 2Darray.o

1Darray.o: 1Darray.c
	gcc -lm -g 1Darray.c -o 1Darray.o ${BSP_FLAGS} ${BSP_LIB}

2Darray.o: 2Darray.c
	gcc -lm -g 2Darray.c -o 2Darray.o ${BSP_FLAGS} ${BSP_LIB}

1D:	1Darray.o
	bsub -oo 1Dlog.log -n ${N} -R"span[ptile=${N}]" ./1Darray.o ${N} ${rows} ${cols}

2D:	2Darray.o
	bsub -oo 2Dlog.log -n ${N} -R"span[ptile=${N}]" ./2Darray.o ${N} ${rows} ${cols}

.PHONY: clean
clean:
	-rm *.o *.log
