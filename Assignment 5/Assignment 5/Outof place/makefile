include ${PARALLEL}cilk.mk
FLAGS=-Icommon

multiply: multiply.cilk
	${CILK} ${CILK_PROFILE} ${FLAGS} ${CILK_FLAGS} multiply.cilk -o multiply

run: multiply
	bsub -o logs/outplace.log -n ${THREADS} -R"span[ptile=16]" ./multiply --stats 1 --nproc ${THREADS} ${N} ${THREADS}

.PHONY: clean
clean:
	rm multiply
