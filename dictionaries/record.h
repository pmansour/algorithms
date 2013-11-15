// record.h
// awirth
// jul 2012
// comp20003


typedef struct rec{
	char key[MAXSTR+1];
	int value;
} record;

void
record_to_string(char *s, record *r);

void
set_key(record *r, char *k);

char *
get_key(record *r);

void
set_value(record *r, int v);

int
get_value(record *r);

record *
make_record(char *k, int v);

int
comp_key(char *s, char *t);

void
remove_record(record *r);
