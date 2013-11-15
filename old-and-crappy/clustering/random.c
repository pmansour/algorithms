#include <stdlib.h>
#include <time.h>
#include "random.h"

int random(int min, int max) {
    // is this the first time this function's
    // been called in this program?
    static int first = 1;
    // if it is, then seed the generator first
    if(first) {
	srand(time(NULL));
	first = 0;
    } 
    return rand() % (max - min + 1) + min;
}
