/*
 * Light-weight Benchmarking Framework
 * Author: Antoine El Hokayem <ape00@mail.aub.edu>
 */
#ifndef BENCH_H
#define BENCH_H 1
#include <stack/stack.h>

/*
 * A measure for benchmarking
 * <out> 	: output stream to print
 * <prefix> 	: prefix the printing with this string
 * <data>	: dummy pointer to allocate data needed by the benchmark measure
 * <onpush> 	: function to execute when the metric is set
 * 		  use the <data> pointer to store data between push and pop like start time
 * <onpop>	: function to execute when the metric is requested to output
 *		Note: make sure to deallocate the data pointer if set on onpush
 */
struct benchmeasure {
	FILE * out;
	char * prefix;
	void * data;
	void (* onpush) (struct benchmeasure *);
	void (* onpop)	(struct benchmeasure *);
};
typedef struct benchmeasure BenchMeasure;


/*
 * Stack containing all the active benchmark measures
 */
List * benchList = NULL; 


/*
 * Push a benchmark onto the stack (indicate the beginning of a benchmark)
 * If measure needs to be constructed for every call consider using <bench_mark> only
 * Triggers <onpush> in measure
 * <measure> : measure needed to be marked/unmarked
 */
void bench_mark_measure(BenchMeasure * measure)
{
	push(&benchList, (void *) measure); 
	measure->onpush(measure);	
}
/*
 * Create and push a benchmark onto the stack
 * Supplying the creation function, this makes it easier to call successfully
 * different benchmarks of the same type
 * <construct> : the function that should create a new BenchMeasure instance
 */
void bench_mark( BenchMeasure * (* construct) (void))
{
	bench_mark_measure(construct());	
}

/*
 * Mark the end of the last benchmark
 * Trigger <onpop> on the last measure inserted
 */
void bench_unmark()
{
	BenchMeasure * measure;
	measure = (BenchMeasure *) pop(&benchList);
	measure->onpop(measure);
	free(measure);
}
/*
 * Mark the end of all active benchmarks
 */
void bench_unmark_all()
{
	while(!empty(benchList))	bench_unmark();
}

#endif
