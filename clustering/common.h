// common.h
// common functions for clustering algorithms
// pmansour 2012

#define INITIAL_CLUSTER	-1
#define NOISE_CLUSTER	-2

/**
  * work out the distances between all the points in the graph
  *
  * -> distances is a multi-dimensional array of ld that has
  *    every point's distance from every other point.
  *    it should already be allocated memory using
  *	distances = smalloc(n * sizeof(ld *));
  * -> ps is the points array we're initially given
  * -> n is the number of points we're initially given
  */
void get_distances(ld **distances, point ps[], int n);

/**
  * work out all the neighbours of each point
  *
  * -> neighbours is a 2-dimensional array of every point's
  *    neighbours
  * -> num_neighbours is an array of integers specifying
  *    how many neighbours each point has
  * -> distances is a 2-dimensional array of distances between
  *    every combination of points
  * -> n is the number of points in the graph
  * -> epsilon is the maximum distance between a point and its
  *    neighbours
  */
void get_neighbours(int **neighbours, int *num_neighbours, ld **distances, int n, ld epsilon);

/**
  * add p and its neighbours to the cluster at cluster_number if
  * they satisfy the requirements and are not already in another
  * cluster.
  *
  * -> p is the index of the point we're looking at
  * -> cluster_number is the cluster we want to add p to
  * -> cs is the array mapping points to clusters
  * -> neighbours is a 2-dimensional array of every point's
  *    neighbours
  * -> num_neighbours is an array of integers specifying how
  *    many neighbours each point has
  * -> epsilon is the maximum distance between a point and its
  *    neighbours
  * -> min_pts is the minimum number of neighbours a point should
  *    have to be considered a dense point (as in its neighbours
  *    should also be added)
  */
int expand_cluster(int p, int cluster_number, int cs[], int **neighbours, int *num_neighbours,
		    ld epsilon, int min_pts);

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
