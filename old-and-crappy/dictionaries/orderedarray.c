//orderedarray.c
// awirth
// jul 12

#include <stdio.h>
#include "utils.h"
#include "record.h"
#include "dict.h"
#include "smalloc.h"
#include "report.h"

#define MAX 1000
typedef struct arr{
        record *rps[MAX]; // it is pointers to records now (sorry)
        int count;      // how many records are in this array?
} orarray;

int
locate(orarray *ra, char *key);

// empty dictionary
void *
initialize(void)
{
	orarray *ora;
	ora = (orarray *) smalloc(sizeof(orarray));
	return (void *) ora;
}


// the locate function uses binary search
// returns position if key already present
// if absent, then if key should be in position i,
// returns -(i+1)
int
locate(orarray *ora, char *key)
{
	int l,r,m,n;
	record **rps = ora->rps;	// array of pointers to records
	char *rkey;

	n = ora->count;
	l = 0, r = n;

	while(l+1 < r){
		m = l + (r-l)/2;
		rkey = get_key(rps[m]);
		// compare middle record with our search key
		if(comp_key(key,rkey) < 0){
			r = m;
			continue;
		}
		if(comp_key(key,rkey) > 0){
			l = m+1;
			continue;
		}
		return m;
	}
	if(l+1 == r){
		// just one item left to search
		rkey = get_key(rps[l]);
		if(comp_key(key,rkey) < 0){
			return -(l+1);
		}
		if(comp_key(key,rkey) > 0){
			return -(l+2);
		}
		return l;
	}
	
	// if l == r, no items left
	return -(l+1);
}


int
insert(void *ds, char *k, int v, FILE *output)
{
	orarray *ora = (orarray *)ds;
	record *rnew;
	int pos;
	int i;
	int old;

	if(ora == NULL){
		fprintf(stderr,"Error: dictionary not initialized\n");
		return FALSE;
	}

	if(ora->count == MAX){
		fprintf(stderr,"Error: Dictionary full\n");
		return FALSE;
	}
	//check where to insert
	pos = locate(ora,k);

	if(pos < 0){
		// if not already present, use negative quantity
		pos = -pos-1; // special code

		// create new record
		rnew = make_record(k,v);

		// shuffle all higher pointers up
		for(i = ora->count; i > pos; i--){
			ora->rps[i] = ora->rps[i-1];
		}
		ora->rps[pos] = rnew;
		ora->count++;
		if(output!=NULL){
			report_insert(FALSE,k,NOTFOUND,v,output);
		}
	} else {
		old = get_value(ora->rps[pos]);
		set_value(ora->rps[pos],v);
		if(output!=NULL){
			report_insert(TRUE,k,old,v,output);
		}
	}
	return TRUE;
}

void
destroy(void *ds, char *key)
{
	orarray *ora = (orarray *)ds;
	record **rps = ora->rps;
	record *doomed;

	int pos,i;
	int last = ora->count-1;

	pos = locate(ora,key);
	if(pos < 0){
		fprintf(stderr,"Warning: no record to destroy with key:\n%s\n",
			key);
		return;
	}
	// pointer to record to be deleted
	doomed = rps[pos];
	for(i=pos;i<last;i++){
		rps[i] = rps[i+1];
	}
	// this could be more efficient
	remove_record(doomed);	// maybe 

	ora->count--;
}

int
find(void *ds, char *k, int *v)
{
	orarray *ora = (orarray *)ds;
	int pos;
	
	pos = locate(ora,k);
	if(pos < 0){
		return FALSE;
	}
	// retrieve the value of the record of interest
	*v = get_value(ora->rps[pos]);
	return TRUE;
}


void
test_dump(void *ds)
{
	orarray *ora = (orarray *) ds;
	int i;
	char s[MAXSTR*2];

	printf("--- begin dump ---\n");
	for(i=0;i<ora->count;i++){
		record_to_string(s,ora->rps[i]);
		printf("%5d: %s\n",i,s);
	}
	printf("--- end dump ---\n");
}
