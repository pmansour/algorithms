#include <stdio.h>
#include <stdlib.h>
#include "smalloc.h"

#define MAX_STR 10

#define FRST_CHR 65	// A
#define LAST_CHR 90	// Z

void next_rnd(char *);
void next_ord(char *);
void prev_ord(char *);

/**
  * generates a random string
  *
  */
void next_rnd(char *out) {
    // for the length of the string:
    for(int i = 0; i < MAX_STR; i++) {
	// add a random character between first and last
	out[i] = rand() % (LAST_CHR - FRST_CHR + 1) + FRST_CHR;
    }

    // terminate the string
    out[MAX_STR] = '\0';
}
/**
  * generate ordered strings
  * based on code by po chang
  *
  */
void next_ord(char *out) {
    int j = MAX_STR;
    // increment the last character of the string
    out[j - 1]++;
    // if there is anything past the limit: reset it and increment the prev char
    while(j-- > 1) {
	if(out[j] > LAST_CHR) {
	    out[j] = FRST_CHR;
	    out[j - 1]++;
	}
    }
}
void prev_ord(char *out) {
    int j = MAX_STR;
    // decrement the last character of the string
    out[j - 1]--;
    // if there is anything past the limit: reset it and increment the prev char
    while(j-- > 1) {
	if(out[j] < FRST_CHR) {
	    out[j] = LAST_CHR;
	    out[j - 1]--;
	}
    }
}


/**
  * the main function. needs at least 1 arg.
  * 
  * inputs:
  *	[0] the program name
  *	[1] the filename to export to
  *	[2] the number of inputs to generate
  *	[3] should the keys be ordered?
  *	[4] ascending or descending?
  */
int main(int argc, char** argv) {
    char    *filename;
    int	    n		= 25;
    int	    ordered	= 0;
    int	    asc		= 1;
    switch(argc) {
	case 5:
	    asc		= atoi(argv[4]);
	    // fallthrough
        case 4:
	    ordered	= atoi(argv[3]);
	    // fallthrough
	case 3:
	    n		= atoi(argv[2]);
	    // fallthrough
	case 2:
	    filename	= argv[1];
	    break;
	// if we're not given the right number of args
	default:
	    printf("Incorrect number of arguments!\n");
	    exit(0);
	    break;
    }

    // initialize the original array
    char *str = smalloc((MAX_STR + 1) * sizeof(char));
    if(ordered) {
	for(int i = 0; i < MAX_STR; i++)
	    str[i] = asc ? FRST_CHR : LAST_CHR;
    } else {
	next_rnd(str);
    }

    // write the required inputs to file
    FILE *fp = fopen(filename, "w");
    for(int i = 0; i < n; i++) {
	// print the current string
	fprintf(fp, "%s\n", str);
	// get the next string
	if(ordered) {
	    if(asc) {
		next_ord(str);
	    } else {
		prev_ord(str);
	    }
	} else {
	    next_rnd(str);
	}
    }
    fclose(fp);

    // return success
    return 0;
}
