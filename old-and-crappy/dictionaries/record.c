// record.c
// awirth
// jul 2012
// comp20003

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "smalloc.h"
#include "record.h"

#define INTWIDE 8

//instantiate a record
record *
make_record(char *key, int value)
{
	record *r;

	r = smalloc(sizeof(record));
	set_value(r,value);
	set_key(r,key);

	return r;
}

//dump the record as a string
void
record_to_string(char *s,record *r)
{
	snprintf(s,MAXSTR+INTWIDE+2,"%-*s %*d",MAXSTR,r->key, INTWIDE,
		r->value);
}

void
set_key(record *r, char *k)
{
	strncpy(r->key, k, MAXSTR+1);
}

// the danger with this is that it is a direct pointer to the string, not
// to a copy of the string
char *
get_key(record *r){
	return r->key;
}

void
set_value(record *r, int v)
{
	r->value = v;
}

int
get_value(record *r)
{
	return r->value;
}

int
comp_key(char *s, char *t)
{
	int ret;
	ret = strncmp(s,t,MAXSTR);
	return ret;
}

void
remove_record(record *r)
{
	free(r);
}
