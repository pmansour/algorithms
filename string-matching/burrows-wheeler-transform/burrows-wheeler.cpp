/***************************************************************************
 * An implementation of the burrows-wheeler transform in C.
 *
 * Peter Mansour, 2013.
 **************************************************************************/

#include <algorithm>
#include <functional>
#include "burrows-wheeler.h"

using namespace std;

// assuming \0 is lower than all other chars
int compare_ints(char *string, const void *a, const void *b) {
    char ca = string[*(int *) a];
    char cb = string[*(int *) b];

    if(ca == cb) {
	// two terminator chars are equal
	if(ca == '\0') {
	    return 0;
	// if two (non-terminator) chars are equal, check the next char
	} else {
	    return compare_ints(
		string,
		(void *) (((int *) ca) + 1),
		(void *) (((int *) cb) + 1)
	      );
	}
    // if a is a terminator char, then it's lower than b
    } else if(ca == '\0') {
	return -1;
    // if b is a terminator char, then it's lower than a
    } else if(cb == '\0') {
	return 1;
    // from here on it's just normal "compare" functionality
    } else if(ca > cb) {
	return 1;
    } else {
	return -1;
    }
}

int *BWT::generate_suffix_array() {
    int *suffix;

    // initialize the suffix array
    suffix = new int[this->len];

    // add all the indices to it
    for(int i = 0; i < this->len; i++) {
	// at this point they're in order
	suffix[i] = i;
    }

    // return the suffix array
    return suffix;
}

char *BWT::transform() {
    int *suffix;
    char *transform;
    int otherend;

    // start by getting the suffix array
    suffix = this->generate_suffix_array();

    // then sort it
    // (we need a comparison function that's specific for this string)
    using namespace std::placeholders;
    auto f = bind(compare_ints, this->string, _1, _2);
    sort(suffix, suffix + this->len, f);

    // allocate memory for the new string
    transform = new char[this->len];

    // fill it with the other ends of the sorted array
    for(int i = 0; i < this->len; i++) {
	// work out the index at the other end of this index
	otherend = suffix[i] - 1;
	if(otherend < 0) {
	    otherend += this->len;
	}

	// set the char in the transform array to the char at the other end
	transform[i] = this->string[otherend];
    }

    // return the transformed string
    return transform;
}
