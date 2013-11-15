// basic dictionary data structure header
// awirth
// jul 09, 12
// comp20003

void *
initialize(void);

int
insert(void *ds, char *k, int v, FILE *output);

void
destroy(void *ds, char *key);

int
find(void *ds, char *key, int *v); 

void
test_dump(void *ds);
