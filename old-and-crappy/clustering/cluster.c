#ifndef LOADED_HEADERS
    #include <stdio.h>
    #include "utils.h"
    #include "smalloc.h"

    #define LOADED_HEADERS
#endif	/* LOADED_HEADERS */

#include "cluster.h"

// a cluster
typedef struct {
    void *parent;
    int number;
    int rank;
} cluster;

void *create_cluster(int n) {
    // allocate memory for the cluster
    cluster *new    = (cluster *) smalloc(sizeof(cluster));
    // initially it has no parent
    new->parent	    = NULL;
    // give it an identifier (a number)
    new->number	    = n;
    // its rank is initially 0
    new->rank	    = 0;
    // return the created cluster
    return (void *) new;
}

void *find(void *scluster) {
    // cast the given subcluster
    // into a workable format
    cluster *subcluster = (cluster *) scluster;
    // while this is not a parent,
    // keep climbing up the tree
    while(subcluster->parent != NULL) {
	subcluster = (cluster *) subcluster->parent;
    }
    return (void *) subcluster;
}

int merge_clusters(void *c1, void *c2) {
    // find the roots of both clusters
    cluster *root1 = (cluster *) find(c1);
    cluster *root2 = (cluster *) find(c2);

    // if they both have the same root, 
    // then they're both in the same
    // cluster, so ignore them
    if(root1 == root2) {
	return 0;
    }
    // if root1 is taller than root2,
    // then attach root2 to root1
    // for efficiency
    if(root1->rank > root2->rank) {
	root2->parent = (void *) root1;
    // otherwise, attach root1 to
    // root2
    } else {
	root1->parent = (void *) root2;
	// if both roots have the same
	// rank now, then increment
	// root2's rank
	if(root1->rank == root2->rank) {
	    root2->rank++;
	}
    }
    // return a success
    return 1;
}

int get_number(void *given_cluster) {
    // convert the cluster into a workable format
    cluster *c = (cluster *) given_cluster;
    // return the cluster number
    return c->number;
}
