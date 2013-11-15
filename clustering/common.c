// common.c
// common functions for clustering
// pmansour 2012


#ifndef LOADED_HEADERS
    #include <stdio.h>
    #include "utils.h"
    #include "smalloc.h"
    #include "point.h"

    #define LOADED_HEADERS
#endif	/* LOADED_HEADERS */

#include "common.h"

void get_distances(ld **distances, point ps[], int n) {
    // for each point i
    for(int i = 0; i < n; i++) {
	// allocate memory for this row of the distances
	// array
	distances[i] = (ld *) smalloc(sizeof(ld) * n);
	// for each point j
	for(int j = 0; j < n; j++) {
	    // we don't care about i's distance from itself
	    if(i == j) {
		continue;
	    }
	    // save "i"'s distance from j
	    distances[i][j] = dist(ps[i], ps[j]);
	}
    }
}

void get_neighbours(int **neighbours, int *num_neighbours, ld **distances, int n, ld epsilon) {
    for(int i = 0; i < n; i++) {
	// initially the point i has 0 neighbours, but there is space for 1 neighbour
	num_neighbours[i] = 0;
	neighbours[i] = (int *) smalloc(sizeof(int) * n);
	// go through all the other points to find its neighbours
	for(int j = 0; j < n; j++) {
	    // i can't be its own neighbour!
	    if(i == j) {
		continue;
	    }

	    // if j is within epsilon of i
	    if(distances[i][j] <= epsilon) {
		// add j as a neighbour of i
		neighbours[i][num_neighbours[i]] = j;
		// increment the number of i's neighbours
		num_neighbours[i]++;
		/*// allocate space for another neighbour
		neighbours[i] = srealloc(neighbours[i], num_neighbours[i] * sizeof(int));*/
	    }
	}
    }
}

int expand_cluster(int p, int cluster_number, int cs[], int **neighbours, int *num_neighbours,
		    ld epsilon, int min_pts) {
    // if p is already part of another cluster, then ignore it and return
    if(cs[p] != INITIAL_CLUSTER) {
	return 0;
    }

    // if p has enough neighbours, add them to the cluster as well
    if(num_neighbours[p] >= min_pts) {
	// add p to the cluster at cluster_number
	cs[p] = cluster_number;
	// add the neighbours to the cluster as well
	for(int i = 0; i < num_neighbours[p]; i++) {
	    expand_cluster(i, cluster_number, cs, neighbours, num_neighbours, epsilon, min_pts);
	}
    } else {
	// add p to the cluster at cluster_number
	cs[p] = cluster_number;
    }

    // return a success
    return 1;
}
