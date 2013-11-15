// array.c
// awirth
// jul 2012
// comp20003

#include <stdio.h>
#include "utils.h"
#include "record.h"
#include "dict.h"
#include "report.h"
#include "smalloc.h"

#define MAX 1000

typedef struct arr{
        record rs[MAX]; // hard-wired, with records, not pointers to records
        int count;      // how many records are in this array?
} rarray;

int
locate(rarray *ra, char *key);

// locate by linear scan through array of records
int
locate(rarray *ra, char *key)
{
	int i;
	for(i=0;i<ra->count;i++){
		if(comp_key(get_key(&ra->rs[i]),key) == 0){
			return i;
		}
	}
	return NOTFOUND; //not found
}

// create empty dictionary
void *
initialize()
{
	rarray *ra;
	ra = (rarray *) smalloc(sizeof(rarray));

	return (void *) ra;
}

int
insert(void *ds, char *k, int v, FILE *output)
{
	rarray *ra = (rarray *)ds;
	record *rnew;
	int pos;
	int old;

	if(ra == NULL){
		fprintf(stderr,"Error: dictionary not initialized\n");
		return FALSE;
	}

	if(ra->count == MAX){
		fprintf(stderr,"Error: Dictionary full\n");
		return FALSE;
	}
	//check update
	pos = locate(ra,k);


	if(pos == NOTFOUND){
		// put new record in last position
		rnew = &ra->rs[ra->count];
		set_key(rnew,k);
		set_value(rnew,v);
		ra->count++;
		if(output != NULL){
			report_insert(FALSE, k, NOTFOUND, v,output);
		}
	} else {
		old = get_value(&ra->rs[pos]);
		set_value(&ra->rs[pos],v);
		if(output != NULL){
			report_insert(TRUE, k, old, v,output);
		}
	}
	return TRUE;
}

// delete a record
void
destroy(void *ds, char *key)
{
	rarray *ra = (rarray *)ds;
	record *rs = ra->rs;

	int pos;
	int last = ra->count-1;

	pos = locate(ra,key);
	if(pos == NOTFOUND){
		fprintf(stderr,"Warning: no record to destroy with key:\n%s\n",
			key);
		return;
	}
	// move data from last position to the one that need to be removed
	// this could be more efficient
	set_key(&rs[pos],get_key(&rs[last]));
	set_value(&rs[pos],get_value(&rs[last]));

	// and drop count to effective remove what was the last record
	ra->count--;
}

int
find(void *ds, char *k, int *v)
{
	rarray *ra = (rarray *)ds;
	int pos;
	
	pos = locate(ra,k);
	if(pos == NOTFOUND){
		return FALSE;
	}
	// place in v the value that is in the record of interest
	*v = get_value(&ra->rs[pos]);
	return TRUE;
}

void
test_dump(void *ds)
{
	rarray *ra = (rarray *) ds;
	int i;
	char s[MAXSTR*2];

	// print a string for each record
	printf("--- begin dump ---\n");
	for(i=0;i<ra->count;i++){
		record_to_string(s,&ra->rs[i]);
		printf("%5d: %s\n",i,s);
	}
	printf("--- end dump ---\n");
}
