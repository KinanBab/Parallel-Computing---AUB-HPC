include ${PARALLEL}mbsp.mk
N=8

run-partial: partial
	bsub -oo runs/log${SIZE}.log -n ${N} -R"span[ptile=${N}]" ./partial ${N} ${SIZE}

partial: partialsums-bsp.c
	gcc -lm -g partialsums-bsp.c -o partial ${BSP_FLAGS} ${BSP_LIB}

.PHONY: clean
clean:
	-@rm partial
	-@rm *summary*
	-@rm runs/*.log
