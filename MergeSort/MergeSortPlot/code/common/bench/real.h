#ifndef BENCH_REAL
#define BENCH_REAL 1
#include <bench/bench.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
/*
 * Realtime benchmarking
 */

//Get current time in double precision
double realtime_parse() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec*1e-6;
}

//Start of realtime benchmark
void realtime_mark(BenchMeasure * measure)
{
    double * start = (double *) malloc(sizeof(double));
    (*start) = realtime_parse();
    measure->data = (void *) start;
}
//End of realtime benchmark
void realtime_unmark(BenchMeasure * measure)
{
    double * start = (double *) measure->data;
    double time = realtime_parse() - *start;
    fprintf(measure->out, "%s %f\n", measure->prefix, time);
    free(measure->data);
}

//Create a new realtime measure
BenchMeasure * measure_realtime(FILE * out, const char * prefix)
{
    BenchMeasure * measure = (BenchMeasure *) malloc(sizeof(BenchMeasure));
    measure->out    = out;
    measure->prefix = (char *) prefix;
    measure->onpush = &realtime_mark;
    measure->onpop  = &realtime_unmark;
    return measure;
}

#endif
