// rbt.c
// implementing a red black tree
// skeleton code by awirth
// original implementation in java by robert
// sedgewick and kevin wayne, available at:
// http://algs4.cs.princeton.edu/33balanced
// c implementation by peter mansour
// aug 12

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

#define BLACK 0
#define RED 1
#define ROOT 2

#define DEBUG 0

typedef struct bnoode{
	record r;
	struct bnoode *c[BINARY];
	int color;	// the color of the link to the parent
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

bnode *rec_insert(void *ds, char *k, int v, FILE *output);

int is_red(bnode *n);

// instead of using pointers to pointers,
// we just return a pointer after we're done
bnode *rotate_left(bnode *n);
bnode *rotate_right(bnode *n);

void flip_colors(bnode *n);

// create an empty bst that has a dummy head node
void *
initialize(void)
{
	bnode *dummy; // its right pointer points to the root (when it exists)
	char zero[1] = MINSTRING;
	dummy = (bnode *) smalloc(sizeof(bnode));
	set_key(&dummy->r,zero);	// yes, this is a dummy head node!
	dummy->c[LEFT]	= NULL;
	dummy->c[RIGHT]	= NULL;
	dummy->color	= ROOT;		// root node has its own color

	// debug
	if(DEBUG) {
	    printf("initialized tree: %p\n", (void *) dummy);
	    printf(" dummy key: '%s'\n", zero);
	    printf(" dummy val: %d\n", get_value(&dummy->r));
	    printf("\n");
	}

	return (void *) dummy;
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

// recursive insertion
bnode *rec_insert(void *ds, char *k, int v, FILE *output) {
    bnode *cur = (bnode *) ds;

    // if the current node is null: insert a new one
    if(cur == NULL) {
	bnode *new = (bnode *) smalloc(sizeof(bnode));
	set_key(&new->r, k);
	set_value(&new->r, v);
	new->c[LEFT]	= NULL;
	new->c[RIGHT]	= NULL;
	new->color	= RED;

	// report this
	if(output != NULL){
	    report_insert(FALSE, k, NOTFOUND, v, output);
	}

	// debug
	if(DEBUG) {
	    printf("created new node %s at %p\n", k, (void *) new);
	}

	return new;
    }

    // otherwise: look for where we should put this

    // if the new key < current key: add it to the left
    if(comp_key(k, get_key(&cur->r)) < 0) {
	cur->c[LEFT]	= rec_insert(cur->c[LEFT], k, v, output);
    // if the new key > current key: add it to the right
    } else if(comp_key(k, get_key(&cur->r)) > 0) {
	cur->c[RIGHT]	= rec_insert(cur->c[RIGHT], k, v, output);
    // if the new key == current key: update the value
    } else {
	// report this if needed
	if(output != NULL) {
	    report_insert(TRUE, k, get_value(&cur->r), v, output);
	}
	// update the value
	set_value(&cur->r, v);
    }

    // debug
    if(DEBUG) {
	printf("rec_insert called for '%s'\n", get_key(&cur->r));
    }

    // if this isn't the root node: fix up any leaning issues
    if(cur->color != ROOT) {
	// debug
	if(DEBUG) {
	    printf(" [this is not the root node]\n");
	}

	// if the right is red but the left isn't: rotate left
	if(is_red(cur->c[RIGHT]) && !is_red(cur->c[LEFT])) {
	    cur = rotate_left(cur);
	}
	// if two lefts in a row are red: rotate right
	if(is_red(cur->c[LEFT]) && is_red(cur->c[LEFT]->c[LEFT])) {
	    cur = rotate_right(cur);
	}
	// if both children are red: flip colors
	if(is_red(cur->c[LEFT]) && is_red(cur->c[RIGHT])) {
	    flip_colors(cur);
	}
    }

    // debug
    if(DEBUG) {
	printf("\n");
    }

    // return the node after all the work's been done to it
    return cur;

}

// here we insert a (new) record into the bst
int
insert(void *ds, char *k, int v, FILE *output)
{
    ds = (void *) rec_insert(ds, k, v, output);

    // debug
    if(DEBUG) {
	printf("inserted %s into tree at %p\n", k, (void *) ds);
	printf("complete tree so far:\n");
	test_dump(ds);
	printf("\n");
    }

    return TRUE;
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
	printf("color: %s\n", this->color == RED ? "red" : "black");
        record_to_string(s,&this->r);
	printf("%*s%s\n",offset,"",s);
	if(this->c[LEFT] != NULL){
		dump_it(this->c[LEFT],offset+EXTRA);
	}
	
}

int is_red(bnode *n) {
    return (n != NULL) && (n->color == RED);
}

bnode *rotate_left(bnode *n) {
    // ensure n is a real node and its right child is red
    if(n == NULL || !is_red(n->c[RIGHT]))
	return NULL;

    // debug
    if(DEBUG) {
	printf(" rotating to the left...\n");
    }

    bnode *x		= n->c[RIGHT];
    n->c[RIGHT]		= x->c[LEFT];
    x->c[LEFT]		= n;
    x->color		= n->color;
    n->color		= RED;

    // debug
    if(DEBUG) {
	printf("  x = n->right (%s)\n", get_key(&x->r));
	printf("  n->right = x->left (%s)\n", n->c[RIGHT] != NULL ? get_key(&n->c[RIGHT]->r) : "NULL");
	printf("  x->left = n (%s)\n", get_key(&n->r));
	printf("  x->color = n->color (%d)\n", x->color);
	printf("  n->color = red\n");
    }
 
    // debug
    if(DEBUG) {
	printf(" rotated to the left!\n");
    }

    return x;
}

bnode *rotate_right(bnode *n) {
    // ensure n is a real node and its left child is red
    if(n == NULL || !is_red(n->c[LEFT]))
	return NULL;

    // debug
    if(DEBUG) {
	printf(" rotating to the right...\n");
    }

    bnode *x		= n->c[LEFT];
    n->c[LEFT]   	= x->c[RIGHT];
    x->c[RIGHT]		= n;
    x->color		= n->color;
    n->color		= RED;

    // debug
    if(DEBUG) {
	printf("  x = n->left (%s)\n", get_key(&x->r));
	printf("  n->left = x->right (%s)\n", n->c[LEFT] != NULL ? get_key(&n->c[LEFT]->r) : "NULL");
	printf("  x->right = n (%s)\n", get_key(&n->r));
	printf("  x->color = n->color (%d)\n", x->color);
	printf("  n->color = red\n");
    }

    // debug
    if(DEBUG) {
	printf(" rotated to the right!\n");
    }

    return x;
}

void flip_colors(bnode *n) {
    // ensure n and its two children aren't NULL
    if(n == NULL || n->c[LEFT] == NULL || n->c[RIGHT] == NULL)
	return;

    // debug
    if(DEBUG) {
	printf(" flipping colors...\n");
    }

    n->color	    = RED;
    n->c[LEFT]->color   = BLACK;
    n->c[RIGHT]->color  = BLACK;

    // debug
    if(DEBUG) {
	printf("  %s->color = red\n", get_key(&n->r));
	printf("  %s->color = black\n", get_key(&n->c[LEFT]->r));
	printf("  %s->color = black\n", get_key(&n->c[RIGHT]->r));
    }

    // debug
    if(DEBUG) {
	printf(" flipped colors!\n");
    }
}
