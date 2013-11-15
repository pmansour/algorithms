// stopwatch.c

// recording when function starts and stops

// awirth
// jul 09

#include <time.h>

#include "stopwatch.h"

#define EMPTY -1

static time_t start_time = EMPTY;
// not sure that having this value is important.

void
start(void)
{
	start_time = time(NULL);
}

int
stop(void)
{
	time_t stop_time = time(NULL);
	int d = difftime(stop_time, start_time);
	start_time = EMPTY;
	return d;
}
