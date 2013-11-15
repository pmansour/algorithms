// test_all.c
// should perform some tests
// on different implementations
// and time them.
// pmansour

#include <stdio.h>
#include <stdlib.h>
#include "time.h"

// the max buffer size we use
#define MAX_BUF 50

// the file we will output to
#define OUT_FILE "out"

// prefixes of generated input files
#define ASC_FILE "in_asc"
#define DSC_FILE "in_dsc"
#define RND_FILE "in_rnd"

// the number of times we will test each input size (and average)
#define TRIES_PER_INPUT 1

/**
  * main()
  *
  * tests the files each implementation
  * and puts output in a file called "out"a
  *
  */
int main(int, char **);

/**
  * generate()
  *
  * inputs:
  *	int n	    : the number of inputs to generate
  *	char *suf   : the suffix to add to the filenames
  *
  * this function generates inputs using
  * the "gen_all" script. these should include
  * ascending, descending, and random inputs.
  *
  */
void generate(int n, char *suf);

/**
  * rm_all_inp()
  *
  * removes all the input files in the
  * current directory.
  *
  */
void rm_all_inp(void);

/**
  * do_test()
  *
  * tests a given program with the given input filename and suffix.
  *
  * returns the average time taken per try.
  *
  */
long do_test(char *prog, const char *file, char *suf);

int main(int argc, char **argv) {
    FILE *out;
    long time;
    char suffix[MAX_BUF];
    // the input sizes we will be testing
    long inputs[4]	= { 10, 100, 1000, 10000 };
    /*long inputs[7]	= { 10, 100, 1000, 10000, 100000, 1000000,
							     10000000 };*/
    size_t c_inputs	= sizeof(inputs) / sizeof(inputs[0]);
    // the programs we will be testing
    char progs[3][8]	= { "testl", "testb", "testrbt" };
    size_t c_progs	= sizeof(progs) / sizeof(progs[0]);


    // open the output file for writing
    out = fopen(OUT_FILE, "w");

    // generate all the inputs first
    for(int i = 0; i < c_inputs; i++) {
	// make the suffix
	snprintf(suffix, MAX_BUF, "%ld", inputs[i]);
	// generate the inputs
	generate(inputs[i], suffix);
    }

    // test each program
    for(int j = 0; j < c_progs; j++) {
        // write to the output file
        fprintf(out, "%s:\n", progs[j]);

	// test it for each input size
	for(int i = 0; i < c_inputs; i++) {
	    // generate the suffix
	    snprintf(suffix, MAX_BUF, "%ld", inputs[i]);
	    // write to the output file
	    fprintf(out, "\t%s inputs:", suffix);

	    // test the ascending inputs and write the avg time
	    time = do_test(progs[j], ASC_FILE, suffix);
	    fprintf(out, " %ldms (asc)", time);

	    // test the descending inputs and write the avg time
	    time = do_test(progs[j], DSC_FILE, suffix);
	    fprintf(out, " %ldms (dsc)", time);

	    // test the random inputs and write the avg time
	    time = do_test(progs[j], RND_FILE, suffix);
	    fprintf(out, " %ldms (rnd)", time);
    
	    // start a new line
	    fprintf(out, "\n");
	}
    }

    // delete all the input files we created
    rm_all_inp();

    // close the output file
    fclose(out);

    // finish successfully
    return 0;
}

void generate(int n, char *suf) {
    // build the command
    char buf[MAX_BUF];
    snprintf(buf, MAX_BUF, "gen_all %d %s", n, suf);
    // execute it
    system(buf);
}

void rm_all_inp(void) {
    system("rm -f in_*");
}

long do_test(char *prog, const char *file, char *suf) {
    // build the command
    char buf[MAX_BUF];
    snprintf(buf, MAX_BUF, "%s %s_%s", prog, file, suf);
    // start a timer
    elapsed_time();
    // execute it the required number of times
    for(int i = 0; i < TRIES_PER_INPUT; i++) {
	system(buf);
    }
    // return an average of the time for one try
    return elapsed_time() / TRIES_PER_INPUT;
}
