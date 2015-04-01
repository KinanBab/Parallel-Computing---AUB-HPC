include ${PARALLEL}mbsp.mk
N=8

run-countsort: countsort
	bsub -oo runs/log${SIZE}.log -n ${N} -R"span[ptile=${N}]" ./countsort ${N} ${SIZE}

countsort: countsort-bsp.c
	gcc -lm -g countsort-bsp.c -o countsort ${BSP_FLAGS} ${BSP_LIB}

.PHONY: clean
clean:
	-@rm partial
	-@rm *summary*
	-@rm runs/*.log
