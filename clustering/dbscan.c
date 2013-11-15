// dbscan.c
// COMP20003 project 2
// 2012s2

#ifndef LOADED_HEADERS
    #include <stdio.h>
    #include "utils.h"
    #include "smalloc.h"
    #include "point.h"

    #define LOADED_HEADERS
#endif	/* LOADED_HEADERS */

#include "clusterings.h"
#include "common.h"

void
cluster(point ps[], int n, int k, int cs[])
{
    int current_cluster;	// the cluster number we're up to
    ld **distances;		// distances between points
    int **neighbours;		// neighbours of a point
    int *num_neighbours;	// number of neighbours of a point

    // initialize the distances array
    distances = (ld **) smalloc(n * sizeof(ld *));
    get_distances(distances, ps, n);

    // initialize the neighbours array
    neighbours = (int **) smalloc(n * sizeof(int *));
    num_neighbours = (int *) smalloc(n * sizeof(int));
    // assume k is our epsilon
    get_neighbours(neighbours, num_neighbours, distances, n, k);

    // initialize all the points to belong to no cluster
    for(int i = 0; i < n; i++) {
	cs[i] = INITIAL_CLUSTER;
    }

    // go through each point
    current_cluster = 0;
    for(int i = 0; i < n; i++) {
	// if this point forms a new cluster at current_cluster,
	// then increment the current cluster we're up to
	//	assume min_pts = n / k
	if(expand_cluster(i, current_cluster, cs, neighbours, num_neighbours, k, n / k)) {
	    current_cluster++;
	}
    }
}
