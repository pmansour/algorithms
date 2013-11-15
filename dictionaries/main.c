// record.c
// awirth
// jul 2012
// comp20003

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "dict.h"

#define COMMASEP ","	// commas separate tokens
#define SPACESEP " \t\n"

#define DEBUG 0

int
parse_key_value(char *key, int *v);

int
main(int argc, char *argv[])
{
	// allocate some strings to use
	char s[2*MAXSTR];
	char key[2*MAXSTR];
	int i;
	FILE *input;
	FILE *output;

	void *ds;


	if(argc < 2){
		fprintf(stderr,"input file needed\n");
		exit(EXIT_FAILURE);
	}

	// create empty dictionary
	ds = initialize();

	// use first command-line argument as input file name
	input = fopen(argv[1],"r");
	// use second command-line argument (if it exists)  as output file name
	if(argc == 3){
		output = fopen(argv[2],"w");
		if(output == NULL){
			fprintf(stderr,"Aborting, cannot open file: "
				" %s\n",argv[2]);
			exit(EXIT_FAILURE);
		}
	} else {
		output = NULL;
	}

	if(input == NULL){
		fprintf(stderr,"Aborting, cannot open file: %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	//for each line in the input
	for(i=0;fgets(s,MAXSTR+1,input)!=NULL;i++){
		sscanf(s,"%s",key);
		insert(ds,key,i,output);
	}

	// close the file handles
	fclose(input);
	if(output != NULL)
	    fclose(output);

	// debug
	if(DEBUG) {
	    printf("final tree:\n");
	    test_dump(ds);
	}
}
