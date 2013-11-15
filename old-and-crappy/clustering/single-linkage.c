// single-linkage.c
// COMP20003 project 2
// 2012s2

#ifndef LOADED_HEADERS
    #include <stdio.h>
    #include "utils.h"
    #include "smalloc.h"
    #include "point.h"
    #include "cluster.h"

    #define LOADED_HEADERS
#endif	/* LOADED_HEADERS */

#include "random.h"
#include "common.h"
#include "clusterings.h"

#define INITIAL -1

/**
  * find the next shortest edge (distance between 2 points)
  *
  * -> point1 is a pointer (so we can return it) to an int,
  *    which will have the index of the first point in the
  *    edge we choose
  * -> point2 is a pointer (so we can return it) to an int,
  *    which will have the index of the second point in the
  *    edge we choose
  * -> distances is a 2-dimensional array containing distances
  *    between every 2 points in the graph
  * -> n is the number of points (nodes) in the graph
  * -> clusters is an array of pointers to clusters
  *
  */
void next_shortest_edge(int *point1, int *point2, ld **distances, int n, void **clusters);

/**
  * normalize the cluster number from crazy ones (e.g. 23490 then 2 then 902 then -50)
  * to normal ascending integers
  *
  * -> cs is an array mapping points to clusters
  * -> n is the number of points (nodes) in the graph
  * -> clusters is an array of pointers to clusters
  *
  */
void normalize_clusters(int cs[], int n, void **clusters);

//-----------------------------------------------------------------------------------------

void next_shortest_edge(int *point1, int *point2, ld **distances, int n, void **clusters) {
    // the shortest edge so far
    int shortest = INITIAL;
    // go through every distance (edge) possible
    for(int i = 0; i < n; i++) {
	for(int j = 0; j < n; j++) {
	    // if the 2 points are already in the same cluster,
	    // then we can safely ignore them
	    if(find(clusters[i]) == find(clusters[j])) {
		continue;
	    }
	    // if there is no shortest edge yet,
	    // or this edge is shorter than the
	    // shortest edge: then update it
	    if(shortest == INITIAL ||
	       distances[i][j] < shortest) {
		*point1 = i;
		*point2 = j;
		shortest = distances[i][j];
	    }
	}
    }
}

void normalize_clusters(int cs[], int n, void **clusters) {

    // convert the crazy cluster numbers to normal ones

    int index[n];   // index mapping crazy to normal numbers
    int upto = 0;   // what normal number we're up to
    int crazy;	    // a temporary variable that will hold
		    // the crazy (old) number of the current
		    // cluster.

    // initialize the index to map everything to garbage
    for(int i = 0; i < n; i++) {
	index[i] = INITIAL;
    }

    // standardize the cluster numbers as per the index
    for(int i = 0; i < n; i++) {
	// find the crazy number of this cluster
	crazy = get_number(find(clusters[i]));

	// if this is the first time we see this crazy number,
	// map it to the normal number we're up to, then
	// increment the normal number we're up to
	if(index[crazy] == INITIAL) {
	    index[crazy] = upto;
	    upto++;
	}
	// each point's cluster is the normal number that the
	// index maps its crazy number to
	cs[i] = index[crazy];
    }
}

//-----------------------------------------------------------------------------------------

void
cluster(point ps[], int n, int k, int cs[])
{

    ld **distances;	// all the edge weights
    void *clusters[n];	// array of pointers to clusters.
			//  note that the indexes of the array are
			//  the same as the indexes of the points
    int point1, point2;	// temporary variables that should store
			// the next pair of closest points

    // allocate memory for the arrays
    distances	= (ld **) smalloc(sizeof(ld *) * n);

    // pre-calculate all the edge weights (distances
    // between every point and every other point)
    get_distances(distances, ps, n);

    // initialize each cluster to have just 1 point
    for(int i = 0; i < n; i++) {
	clusters[i] = create_cluster(i);
    }

    // keep merging clusters until we're down to k clusters
    for(int num_clusters = n; num_clusters > k; num_clusters--) {
	// find the 2 closest clusters that aren't connected
	next_shortest_edge(&point1, &point2, distances, n, clusters);
	// merge them
	merge_clusters(clusters[point1], clusters[point2]);
    }

    // convert the crazy cluster numbers to normal ones
    normalize_clusters(cs, n, clusters);

}
