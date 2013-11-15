#include <stdio.h>
#include "utils.h"
#include "report.h"

void
report_insert(int found, char *k, int old, int new, FILE *output)
{
	if(found){
		fprintf(output,"Replaced %8d with %8d for key %*s\n",
			old,new,MAXSTR+2,k);
	} else {
		fprintf(output,"Inserted %8d               for key %*s\n",
			new,MAXSTR+2,k);
	}
}
