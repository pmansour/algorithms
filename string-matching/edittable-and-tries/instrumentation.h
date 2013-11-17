#ifndef __INSTRUMENTATION_H_INCLUDED__
#define __INSTRUMENTATION_H_INCLUDED__
#include <cstdlib>

void reset_counters();

void inc_comparisons(int by_how_much = 1);
void inc_ds(int by_how_much = 1);

long long get_comparisons();
long long get_ds();

void print_stats(size_t ds_size);

#endif
