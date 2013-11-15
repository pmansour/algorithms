#include <stdlib.h>
#include <stdio.h>

#define MAX_BUF 50

/**
  * the main function
  *
  * inputs:
  * [0] script name
  * [1] number of inputs to generate
  * [2] suffix to add to generated files
  *
  */
int main(int argc, char **argv) {
    int n		    = 100;  // default number of inputs
    char files[3][MAX_BUF]  = { "in_asc", "in_dsc", "in_rnd" };	// output filenames

    switch(argc) {
	// if we have 2 inputs: add a suffix to the filenames
	case 3:
	    // if we're given a bigger suffix than allowed: trim it
	    if(sizeof(argv[2]) > (MAX_BUF -7)) {
		snprintf(argv[2], MAX_BUF - 7, "%s", argv[2]);
	    }
	    for(int i = 0; i < 3; i++) {
		snprintf(files[i], MAX_BUF, "%s_%s", files[i], argv[2]);
	    }
	    // fallthrough
	// if we have > 1 input: use the given number of inputs
	case 2:
	    n = atoi(argv[1]);
	    break;
    }

    // a buffer to hold the command strings
    char buf[MAX_BUF];

    // generate the files

    // ordered ascending
    snprintf(buf, MAX_BUF, "./generate %s %d 1 1", files[0], n);
    system(buf);
    // ordered descending
    snprintf(buf, MAX_BUF, "./generate %s %d 1 0", files[1], n);
    system(buf);
    // random
    snprintf(buf, MAX_BUF, "./generate %s %d 0",   files[2], n);
    system(buf);

    // return success
    return 0;
}
