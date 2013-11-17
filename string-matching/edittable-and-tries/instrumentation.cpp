#include <cmath>
#include <string>
#include <cstring>
#include <iostream>
#include "instrumentation.h"

long long num_comparisons;
long long num_data_structures;

// reset the local numbers to 0
void reset_counters() {
    num_comparisons	= 0l;
    num_data_structures	= 0l;
}

// increment the numbers by 1
void inc_comparisons(int by_how_much) {
    num_comparisons += by_how_much;
}
void inc_ds(int by_how_much) {
    num_data_structures += by_how_much;
}

// getters
long long get_comparisons() {
    return num_comparisons;
}
long long get_ds() {
    return num_data_structures;
}

void print_stats(size_t ds_size) {
    const long double KB = pow(2, 10);
    const long double MB = pow(2, 20);
    const long double GB = pow(2, 30);

    long long memory_use_bytes;

    // print the standard stuff
    std::cout << get_comparisons() << " comparisons" << std::endl;
    std::cout << get_ds() << " data structures created" << std::endl;

    // work out the memory use in bytes
    memory_use_bytes = get_ds() * ds_size;

    // format that
    if(memory_use_bytes > GB) {
	std::cout << memory_use_bytes / GB << "GB of ram used" << std::endl;
    } else if(memory_use_bytes > MB) {
	std::cout << memory_use_bytes / MB << "MB of ram used" << std::endl;
    } else if(memory_use_bytes > KB) {
	std::cout << memory_use_bytes / KB << "KB of ram used" << std::endl;
    } else {
	std::cout << memory_use_bytes << "B of ram used" << std::endl;
    }
}
