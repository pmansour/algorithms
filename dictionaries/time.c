#include <sys/time.h>
#include "time.h"

long elapsed_time(void) {
    // the value we will be returning
    long r = 0;
    // the last time the function was called
    static struct timeval t0;
    // the current time
    struct timeval tv;
    gettimeofday(&tv, 0);
    // initialize t0 on first call
    if(!t0.tv_sec || !t0.tv_usec)
	t0 = tv;
    // work out the elapsed time
    r	= (tv.tv_sec  - t0.tv_sec ) * 1000;	// seconds
    r  += (tv.tv_usec - t0.tv_usec) / 1000;	// milliseconds
    // update the last time the function was called
    t0 = tv;
    // return the elapsed time
    return r;
}
