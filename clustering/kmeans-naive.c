// kmeans.c
// COMP20003 project 2
// 2012s2

#include "utils.h"
#include "point.h"
#include "clusterings.h"
#include "random.h"

void
cluster(point ps[], int n, int k, int cs[])
{
	point centers[k];	// the center point of each cluster
	int change;		// any change since the last iteration?
	// the current point
	point c_point;
	ld c_dist;
	// the nearest cluster, and the distance to its center
	int n_cluster;
	ld n_dist;
	// the sum of all of a cluster's points, and the number
	// of points added together
	point sum_points;
	int num_points;

	/* // initialize the cluster means to be random points
	int j = 0;
	while(j < (k - 1)) {
		// choose a random point
		p = ps[random(0, n)];
		// ensure we're not already using it
		
	}*/

	// initialize all the points to be in random clusters
	for(int i = 0; i < n; i++) {
		cs[i] = random(0, k - 1);
	}

	// initialize the cluster centers to be the first k points
	for(int j = 0; j < k; j++) {
		centers[j] = ps[j];
	}

	/*// initialize the cluster means to be random points
	for(int j = 0; j < k; j++) {
		centers[j] = ps[random(0, n - 1)];
	}*/

	// keep refining the clusters until there is no more change
	do {
		// reset the "changed" switch
		change = 0;

		// partition all the points to their nearest clusters
		for(int i = 0; i < n; i++) {
			// the point we're currently at
			c_point		= ps[i];
			// the nearest cluster to this point
			n_cluster	= cs[i];
			n_dist		= dist(c_point, centers[n_cluster]);
			// search for the nearest cluster to this point
			for(int j = 0; j < k; j++) {
				// if the distance to this cluster center
				// is shorter than the shortest distance,
				// then this is the nearest cluster so far
				if( ( c_dist = dist(c_point, centers[j]) ) < n_dist ) {
					// update the nearest cluster
					n_cluster	= j;
					n_dist		= c_dist;
					// note that we performed a change
					change = 1;
				}
			}
			// assign this point to its nearest cluster
			cs[i] = n_cluster;
		}

		// set all the cluster centers to be the mean of their points
		for(int j = 0; j < k; j++) {
			// reset the points summed up
			initialize(&sum_points);
			num_points = 0;
			// go through all this cluster's points
			for(int i = 0; i < n; i++) {
				// if this point belongs to a different
				// cluster: we don't care about it
				if(cs[i] != j) {
					continue;
				}
				// add it to the sum
				add_to(&sum_points, ps[i]);
				num_points++;
			}
			// work out the mean center of the cluster
			divide(&sum_points, num_points);
			// let the cluster center be the mean of all the points
			centers[j] = sum_points;
		}

	} while(change);
}
