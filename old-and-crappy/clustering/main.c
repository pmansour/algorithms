// main.c
// comp20003 project 2
// 2012s2

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "point.h"
#include "smalloc.h"
#include "clusterings.h"


void
test_read(point ps[], int n, int k, FILE *output);

void
print_clustering(point ps[], int n, int k, int cs[], FILE *output);

int
main(int argc, char *argv[])
{
	char s[2*MAXSTR];
	int k,n,i;
	FILE *input=stdin;
	FILE *output=stdout;

	point *ps;
	point p;
	int *cs;

	cs = (int *) smalloc(sizeof(int) * MAX);
	ps = (point *) smalloc(sizeof(point) * MAX);


	// get number of clusters
	fgets(s,MAXSTR+1,input);
	sscanf(s,"%d",&k);
	if(k < 1){
		fprintf(stderr,"Number of clusters must be at least 1\n");
		exit(EXIT_FAILURE);
	}
	if(k > MAXK){
		fprintf(stderr,"Number of clusters must be at most %d\n",
			MAXK);
		exit(EXIT_FAILURE);
	}
	
	//for each line in the input
	for(i=0;fgets(s,MAXSTR+1,input)!=NULL && i < MAX;i++){
		sscanf(s,"%Lf %Lf",&p.x,&p.y);
		ps[i] = p;
	}
	n = i;
	if(n == MAX){
		fprintf(stderr,"Could only read in at most %d points.\n",MAX);
	}
	if(k > n){
		fprintf(stderr,"k (%4d) is greater than n (%8d). Aborting.\n",
			k,n);
		exit(EXIT_FAILURE);
	}

	//test_read(ps,n,k,output);

	cluster(ps,n,k,cs);
	print_clustering(ps,n,k,cs,output);	

	free(cs);
	free(ps);
}

void
test_read(point ps[], int n, int k, FILE *output)
{
	int i,j;
	char str[MAXSTR+1];

	fprintf(output,"k is %8d\n",k);
	for(i=0;i<n;i++){
		point2string(str,ps[i]);
		fprintf(output,"ps[%6d]: %s\n",i,str);
	}
	for(i=0;i<n;i++){
		for(j=i;j<n;j++){
			fprintf(output,"d(ps[%4d],ps[%4d]) = %20.10Lf\n",
				i,j,dist(ps[i],ps[j]));
		}
	}
}

void
print_clustering(point ps[], int n, int k, int cs[],FILE *output)
{
	int i;
	char str[MAXSTR+1];
	for(i=0;i<n;i++){
		point2string(str,ps[i]);
		fprintf(output,"%s  %4d\n",str,cs[i]+1);
	}
}
