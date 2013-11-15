// implements clusters of points
// pmansour 2012

// create a new cluster (number n)
void *create_cluster(int n);

// find the root cluster of a given subcluster
void *find(void *scluster);

// merge 2 given subclusters into 1 cluster, and
// return whether or not they were merged
int merge_clusters(void *cluster1, void *cluster2);

// get the number of a given cluster
int get_number(void *cluster);
