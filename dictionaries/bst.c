// bst.c
// implementing as bst
// awirth
// aug 09

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "utils.h"
#include "record.h"
#include "dict.h"
#include "smalloc.h"
#include "report.h"

#define LEFT 0
#define RIGHT 1
#define MINSTRING ""
#define EXTRA 3
#define BINARY 2

typedef struct bnoode{
	record r;
	struct bnoode *c[BINARY];
} bnode;


typedef struct parentchild{
	bnode *parent;	// pointer to parent node of node of interest
	int which;	// tells us whether it is a left or right child
} pc;

int
locate(pc *p, char *key);

void
find_min(pc *p);

void
set_dummy(pc *p, bnode *dummy);

bnode *
zap(pc *p);

void
dump_it(bnode *this, int offset);

// create an empty bst that has a dummy head node
void *
initialize(void)
{
	bnode *dummy; // its right pointer points to the root (when it exists)
	char zero[1] = MINSTRING;
	dummy = (bnode *) smalloc(sizeof(bnode));
	set_key(&dummy->r,zero);	// yes, this is a dummy head node!
	dummy->c[LEFT] = NULL;
	dummy->c[RIGHT] = NULL;

	return dummy;
}

// find the location of the key: if it is not present, the returned
// pc should have now set to NULL and parent/which set up so that the key
// would be inserted as the 'which' child of parent.
int
locate(pc *p, char *key)
{
	bnode *this = p->parent->c[p->which];	// pointer to the child of
						// interest

	if(this == NULL){		// if we have fallen off the tree
		return FALSE;	// the key is not present, but
				// return this location, where it
			// *should* be
	}
	if(comp_key(key,get_key(&this->r)) < 0){
		p->parent = this;
		p->which = LEFT;
		return locate(p,key);	// in tree
	}
	if(comp_key(key,get_key(&this->r)) > 0){
		p->parent = this;
		p->which = RIGHT;
		return locate(p,key);
	}

	// if we have reached here, then we have found the key
	return TRUE;
}

// here we insert a (new) record into the bst
int
insert(void *ds, char *k, int v, FILE *output)
{
	bnode *dummy = (bnode *) ds;
	pc pw;
	pc *p = &pw;
	bnode *new;
	bnode *found;
	int old;

	set_dummy(p,dummy);
	// find the correct place for insertion
	if(locate(p,k)){
		found = p->parent->c[p->which];
		old = get_value(&found->r);
		set_value(&found->r,v);
		if(output != NULL){
			report_insert(TRUE,k,old,v,output);
		}
	} else {
		// set up the new node, which will be a leaf (NULL children)
		new = (bnode *) smalloc(sizeof(bnode));
		set_key(&new->r,k);
		set_value(&new->r,v);
		new->c[LEFT] = NULL;
		new->c[RIGHT] = NULL;
		// connect the parent to the new node, appropriately
		p->parent->c[p->which] = new;
		if(output != NULL){
			report_insert(FALSE,k,NOTFOUND,v,output);
		}
	}
	return TRUE;
}


// deletes the record corresponding to the given key
void
destroy(void *ds, char *key)
{
	pc pw;
	pc *p = &pw;
	bnode *dummy = (bnode *) ds;

	// find out where the node corresponding to this key is
	set_dummy(p,dummy);
	if(locate(p,key)){
		free(zap(p));
	} else {
		fprintf(stderr, "Couldn't find key %s to destroy.\n",key);
		return;
	}
}

// does the hard work of deleting the record
bnode *
zap(pc *p)
{
	pc mw;
	pc *m = &mw;
	bnode *this;
	bnode *replacement;
	this = p->parent->c[p->which];

	// if has neither child
	if(this->c[LEFT] == NULL && this->c[RIGHT] == NULL){
		// just ignore the node
		p->parent->c[p->which] = NULL;
		// and return it to be freed or reused
		return this;
	}
	// if has only right child
	if(this->c[LEFT] == NULL){
		// connect the parent to the right child
		p->parent->c[p->which] = this->c[RIGHT];
		return this;
	}
	// similarly, if has only left child
	if(this->c[RIGHT] == NULL){
		p->parent->c[p->which] = this->c[LEFT];
		return this;
	}
	// else has both kids

	// so find the minimum node in the right subtree
	m->parent = this;
	m->which = RIGHT;

	// sets up m to refer to the correct part of the tree
	find_min(m);
	replacement = zap(m);

	p->parent->c[p->which] = replacement;
	replacement->c[LEFT] = this->c[LEFT];
	replacement->c[RIGHT] = this->c[RIGHT];

	return this;
}


void
find_min(pc *m)
{
	// if there is a left child
	bnode *this = m->parent->c[LEFT];
	if(this->c[LEFT] != NULL){
		// keep going
		m->parent = this;
		m->which = LEFT;
		find_min(m);
	}
}

// given a key, put the value corrsponding to it in the string s
int
find(void *ds, char *key, int *v)
{
	bnode *dummy = (bnode *) ds;
	pc pw;
	pc *p = &pw;

	set_dummy(p,dummy);
	// find out the appropriate location of where this key should be
	if(locate(p,key)){
		*v = get_value(&p->parent->c[p->which]->r);
		return TRUE;
	} else {
		return FALSE;
	}

}

void
set_dummy(pc *p, bnode *dummy)
{
	p->parent = dummy;
	p->which = RIGHT;
}

void
test_dump(void *ds)
{
	bnode *dummy = (bnode *) ds;
	printf("--- begin dump ---\n");
	if(dummy->c[RIGHT] != NULL){
		dump_it(dummy->c[RIGHT],0);
	}
	printf("--- end dump ---\n");
}

void
dump_it(bnode *this, int offset)
{
	char s[2*MAXSTR];
	if(this->c[RIGHT] != NULL){
		dump_it(this->c[RIGHT],offset+EXTRA);
	}
        record_to_string(s,&this->r);
	printf("%*s%s\n",offset,"",s);
	if(this->c[LEFT] != NULL){
		dump_it(this->c[LEFT],offset+EXTRA);
	}
	
}
