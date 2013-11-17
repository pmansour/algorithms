/***************************************************************************
 * An implementation of the burrows-wheeler transform in C.
 *
 * Peter Mansour, 2013.
 **************************************************************************/

#ifndef __BURROWS_WHEELER_H__
#define __BURROWS_WHEELER_H__

#include <cstring>

// comparison function used for sorting the suffix array
int compare_ints(char *string, const void *a, const void *b);

class BWT {
    char *string;
    int len;

    public:
	// constructor with the string to be BWT'ed
	BWT(char *string) {
	    // save the string we're playing with
	    this->string = string;
	    // save its length (+ 1 for the terminator char)
	    this->len = strlen(string) + 1;
	}

	// initializes an array of ints (indices into string) in suffix
	int *generate_suffix_array();

	// perform the burrows-wheeler transform and get the string
	char *transform();
};

#endif
