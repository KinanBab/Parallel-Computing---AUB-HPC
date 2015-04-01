#ifndef BENCH_CLOCK
#define BENCH_CLOCK 1
#include<bench/bench.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
/*
 * Clocktime Benchmarking
 */

//onpush: mark start
void clocktime_mark(BenchMeasure * measure)
{
    clock_t * start = (clock_t *) malloc(sizeof(clock_t));
    (*start) = clock();
    measure->data = (void *) start;
}
//onpop: print start - end
void clocktime_unmark(BenchMeasure * measure)
{
    clock_t * start = (clock_t *) measure->data;
    clock_t end = clock();
    double time = ((double) (end - *start)) / ((double)CLOCKS_PER_SEC); 
    fprintf(measure->out, "%s %f\n", measure->prefix, time);
    free(measure->data);
}

/*
 * Construct the measure:
 * <out>	output filestream
 * <prefix>	prefix output with this string
 */
BenchMeasure * measure_clocktime(FILE * out, const char * prefix)
{
    BenchMeasure * measure = (BenchMeasure *) malloc(sizeof(BenchMeasure));
    measure->out    = out;
    measure->prefix = (char *) prefix;
    measure->onpush = &clocktime_mark;
    measure->onpop  = &clocktime_unmark;
    return measure;
}

#endif
