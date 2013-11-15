//smalloc.c
//a safer, though simple, version of malloc
// awirth
// jul 09

#include <stdlib.h>
#include <stdio.h>
#include "smalloc.h"


void *
smalloc(size_t size)
{
	void *p = malloc(size);
	if(p == NULL){
		fprintf(stderr,"There was not enough space on the heap. "
		"Aborting.\n");
		exit(EXIT_FAILURE);
	}
	return p;
}

void *srealloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);
    if(p == NULL) {
	fprintf(stderr, "There was not enough space on the heap. Aborting.\n");
	exit(EXIT_FAILURE);
    }
    return p;
}
