#ifndef __MATCHER_H_INCLUDED__
#define __MATCHER_H_INCLUDED__
#include <cstring>
#include "instrumentation.h"

struct SearchQuery {
    char *query_str;
    int query_len;
    int errors_allowed;

    void set(const char *str, int len, int errors) {
	// allocate memory for the query string
	query_str = new char[len];
	// copy it
	strncpy(this->query_str, str, len);
	// set the query length
	this->query_len = len;
	// set the number of errors allowed
	this->errors_allowed = errors;
    }
};

struct SearchResult {
    char *match;
    int match_len;
    int errors;

    SearchResult(int max_size) {
	// allocate memory for match
	match = new char[max_size];

	// to show that there is still no match
	match_len = -1;
    }

    void try_update(char *newmatch, int newlen, int newerrors) {
	// there is at least 1 comparison here
	inc_comparisons();

	// if the new errors is less than the current no. of errors,
	// or if this result is still empty,
	// then update the result with the new match
	if(newerrors < this->errors || this->match_len == -1) {
	    this->match = newmatch;
	    this->match_len = newlen;
	    this->errors = newerrors;
	}
    }

    bool has_value() {
	return match_len != -1;
    }
};

// an "interface" for our data structure trials
class Matcher {
    public:
	// step 1 - load the text into the data structure
	virtual void load_text(char *text, int size) = 0;
	// step 2 - try to find a match for a given query
	virtual SearchResult *execute_query(SearchQuery *query) = 0;

	// for calculating how much ram its taking up
	virtual size_t size() = 0;

	// a destructor
	~Matcher() {};
};

// create a new instance of a superclass of Matcher
Matcher *create_matcher();

#endif
