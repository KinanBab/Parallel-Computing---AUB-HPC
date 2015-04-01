include ${PARALLEL}mbsp.mk

all:  bench product

bench:
	gcc -lm -g bspbench.c 	-o bench ${BSP_LIB} ${BSP_FLAGS}
product:
	gcc -lm -g bspinprod.c 	-o product ${BSP_LIB} ${BSP_FLAGS}

modproduct:
	gcc -lm -g modified_bspinprod.c -o modproduct ${BSP_LIB} ${BSP_FLAGS}


runbench:
	bsub -oo logs/bench_log/sublogs/bench${CORES}.log 	-n ${CORES} -R"span[ptile=${CORES}]" ./bench ${CORES}
runproduct:
	bsub -o logs/product_log/sublogs/product${CORES}.log 	-n ${CORES} -R"span[ptile=${CORES}]" ./product ${CORES} ${N}

runmod: modproduct
	bsub -o logs/mod_product_log/sublogs/product${CORES}.log 	-n ${CORES} -R"span[ptile=${CORES}]" ./modproduct ${CORES} ${N}
