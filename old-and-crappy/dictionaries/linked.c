#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "record.h"
#include "smalloc.h"
#include "dict.h"
#include "report.h"

typedef struct noode{
	struct noode *next;
	record r;
} node;

node *
locate(node *dummy, char *k);

node *
make_node(void);

node *
locate(node *dummy, char *k)
{
	node *prev;
	for(prev = dummy; prev->next != NULL; prev = prev->next){
		if(comp_key(get_key(&prev->next->r),k)==0){
			return prev;
		}
	}
	return NULL;
}

node *
make_node(void)
{
	node *n = (node *) smalloc(sizeof(node));
	return n;
}

void *
initialize(void)
{
	node *dummy = make_node();
	dummy->next = NULL;
	return dummy;
}

int
insert(void *ds, char *k, int v, FILE *output)
{
	node *dummy = (node *) ds;
	node *prev;
	int old;

	if((prev = locate(dummy,k)) == NULL){
		node *new = make_node();
		new->next = dummy->next;
		dummy->next = new;
		set_key(&new->r,k);
		set_value(&new->r,v);
		if(output != NULL){
			report_insert(FALSE,k,NOTFOUND,v,output);
		}
	} else {
		old = get_value(&prev->next->r);
		set_value(&prev->next->r,v);
		if(output != NULL){
			report_insert(TRUE,k,old,v,output);
		}
	}
	return TRUE;
}

void
destroy(void *ds, char *key)
{
	node *dummy = (node *) ds;
	node *doomed;
	node *prev;

	if((prev = locate(dummy,key)) == NULL){
		fprintf(stderr,"Warning: no record to destroy with key:\n%s\n",
			key);
	}
	doomed = prev->next;
	prev->next = doomed->next;
	
	free(doomed);
}

int
find(void *ds, char *key, int *v)
{
	node *dummy = (node *) ds;
	node *prev;
	if((prev = locate(dummy,key)) == NULL){
		return FALSE;
	} else {
		*v = get_value(&prev->next->r);
		return TRUE;
	}
}

void
test_dump(void *ds)
{
	node *dummy = (node *) ds;
	node *prev;
	int i;

	char s[2*MAXSTR];
	printf("--- begin dump ---\n");
	for(prev = dummy, i = 0 ; prev->next != NULL; prev = prev->next, i++){
		record_to_string(s,&prev->next->r);
                printf("%5d: %s\n",i,s);
	}
        printf("--- end dump ---\n");
}
