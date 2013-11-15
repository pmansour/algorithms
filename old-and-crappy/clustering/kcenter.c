// kcenter.c
// COMP20003 project 2
// 2012s2

#ifndef LOADED_HEADERS
    #include <stdio.h>
    #include "utils.h"
    #include "smalloc.h"
    #include "point.h"

    #define LOADED_HEADERS
#endif	/* LOADED_HEADERS */

#include "common.h"
#include "clusterings.h"
#include "random.h"

/**
  * calculate the distance of a point from the rest of the centers
  *
  * -> p is the index of the point we're looking at.
  * -> centers is the array containing the current center points.
  * -> num_centers is an int describing how many meaningful entries
  *    are in centers.
  * -> distances is a 2-dimensional array of all the edges in the graph.
  * -> n is the number of points in the graph.
  *
  */
ld dist_from_centers(int p, int centers[], int num_centers, ld **distances, int n);

ld dist_from_centers(int p, int centers[], int num_centers, ld **distances, int n) {
    // the closest edge between p and centers
    ld closest = INITIAL;
    // go through all the center points
    for(int i = 0; i < num_centers; i++) {
	// if this is the first center point,
	// or if it's the closest to p, then
	// hold on to its distance
	if(closest == INITIAL ||
	   distances[p][centers[i]] < closest) {
	    closest = distances[p][centers[i]];
	}
    }
    // return the distance between p and centers
    return closest;
}
    
void
cluster(point ps[], int n, int k, int cs[])
{
    int centers[k];	// the center points
    ld **distances;	// the distances between points
    // the next 2 variables are used with different meanings
    // in different contexts: in the first,
    // best means furthest point from centers,
    // but in the second, best means closest
    // center to point
    int best_index;	// the current best point/center
    ld best_dist;	// the current best distance

    ld dist_holder;	// temporary variable to hold a distance

    // initialize the distances array
    distances = (ld **) smalloc(n * sizeof(ld *));
    get_distances(distances, ps, n);

    // our first center is a random point!
    centers[0] = random(0, n - 1);

    // look for the furthest points from the
    // current centers and make them centers
    // as well
    //	note: j starts at 1 because we already
    //	      have one of the centers. j is
    //	      actually our "num_centers" variable
    //	      as well.
    for(int j = 1; j < k; j++) {
	// reset the furthest point
	best_index  = INITIAL;
	best_dist   = INITIAL;
	for(int i   = 0; i < n; i++) {
	    // if this is the first point we go
	    // through this round, or if it's
	    // the furthest from centers,
	    // then take note of its index
	    if(best_index   == INITIAL ||
	       (dist_holder = dist_from_centers(i, centers, j, distances, n)) > best_dist) {
		best_index  = i;
		best_dist   = dist_holder;
	    }
	}
	// make the furthest point a center
	centers[j]  = best_index;
    }

    // partition the points into their nearest center cluster
    for(int i = 0; i < n; i++) {
	// reset the closest center
	best_index  = INITIAL;
	best_dist   = INITIAL;
	for(int j = 0; j < k; j++) {
	    // if this is the first center we
	    // look at for this point, or if
	    // it's the closest one to i, then
	    // take note of its index
	    if(best_index   == INITIAL ||
	       distances[i][centers[j]] < best_dist) {
		best_index  = j;
		best_dist   = distances[i][centers[j]];
	    }
	}
	// the point i belongs to the
	// cluster of its closest center
	cs[i] = best_index;
    }
}
