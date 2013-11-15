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

int
parse_key_value(char *key, int *v);

int
main(int argc, char *argv[])
{
	// allocate some strings to use
	char s[2*MAXSTR];
	char key[2*MAXSTR];
	

	// when tokenizing each line, use this pointer
	char *tok;
	char op;
	int v;
	int i;
	FILE *input;

	void *ds;


	if(argc != 2){
		fprintf(stderr,"input file needed\n");
		exit(EXIT_FAILURE);
	}

	// create empty dictionary
	ds = initialize();

	// use command-line argument as file name
	input = fopen(argv[1],"r");

	if(input == NULL){
		fprintf(stderr,"Aborting, cannot open file: %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	//for each line in the input
	while(fgets(s,MAXSTR+1,input)!=NULL){
		// make sure THIS DOESN'T FAIL ON INSUFFICIENT INPUTS!
		tok = strtok(s,COMMASEP);
		if(tok == NULL){
			continue;
			// nothing in this line
		}
		// first non-space character says what kind of operation it is
		for(i=0;isspace(i) && tok[i] != '\0';i++){
		}
		op = tok[i];
		switch(op){
			case 'i':
				//what's the value to store?
				if(parse_key_value(key,&v)==2){
					insert(ds,key,v);
				}
				break;
			case 'd':
				//delete
				if(parse_key_value(key,&v)==1){
					destroy(ds,key);
				}
				break;
			case 'f':
				// can we find the key?
				if(parse_key_value(key,&v)==1){
					if(find(ds,key,&v)){
						printf("Value %8d for key "
							"'%-s'\n",v,key);
					} else {
						printf("No record      for "
						"key '%-s'\n",key);
					}
				}
				break;
			default:
				// do nothing
				break;
		}
		// for testing
		//test_dump(ds);
	}
}

// separate by commas, within an entry, normalize the spaces!
int
parse_key_value(char *key,int *value)
{
	char *tok;
	char k[2*MAXSTR];
	int sofar = 0;
	int ret = 0;
	// grab the key, as the next token in the line
	tok = strtok(NULL,COMMASEP);
	if(tok == NULL){
		fprintf(stderr,"No key supplied!\n");
		return 0;
	}
	strncpy(k,tok,MAXSTR+1);
	tok = strtok(NULL,COMMASEP);
	if(tok == NULL || sscanf(tok,"%d",value)!=1){
		ret = 0;
	} else {
		ret = 1;
	}
	tok = strtok(k,SPACESEP);
	if(tok == NULL){
		fprintf(stderr,"No key supplied!\n");
		return 0;
	}
	do{
		if(sofar != 0){
			key[sofar] = ' ';
			sofar++;
		}
		strncpy(key+sofar,tok,MAXSTR+1);
		sofar += strlen(tok);
	} while((tok = strtok(NULL,SPACESEP)) != NULL);

	// for debugging
	//printf("key is: '%s'\n",key);
	return ret+1;
}
