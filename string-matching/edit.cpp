#include <cstring>
#include <iostream>
#include <algorithm>
#include "matcher.h"
#include "edit.h"
#include "char.h"
#include "instrumentation.h"

#define INIT_POS -1
#define __DEBUG__ false

// the node represented by a given point
#define node(p) (distances[(p)->j][(p)->i])
// the edit distance at a given point, from the distance matrix
#define dist(p) (node(p).distance)

using namespace std;

// simply reverses a string in place
void strrev(char *str, int len) {
    for(int i = 0; i < len / 2; i++) {
	std::swap(str[i], str[len - i - 1]);
    }
}

// empty constructor
EditTable::EditTable() {
}

// destructor
EditTable::~EditTable() {
    // free the rows of the table
    free_rows();
    // free the table itself
    delete[] distances;

    // get rid of the best match
    if(best_match != NULL) {
	delete best_match;
    }
}

// compares the 3 given points based on their given distances, and
// returns the point with the lowest distance
Point *min3(Point *p1, int d1, Point *p2, int d2, Point *p3, int d3) {
    if(d1 < d2) {
	return d1 < d3 ? p1 : p3;
    } else {
	return d2 < d3 ? p2 : p3;
    }
}

// get the point surrounding (j,i) with the minimum distance
Point *EditTable::min_neighbour(int j, int i) {
    // the only valid surrounding for the top-corner is (0,0)
    if(j == 1 && i == 1) {
	return new Point(0, 0);
    // if we're in the top row, we can't go up any higher
    } else if(j == 1) {
	return new Point(j, i - 1);
    // if we're in the leftmost column, we CAN go more left
    // (since it just means we're starting the match from there)
    } else if(i == 1) {
	return new Point(j, 0);
    // otherwise,
    } else {
	// 3 comparisons here
	inc_comparisons(3);

	return min3(
		// just above it
		new Point(j - 1, i),
		    distances[j - 1][i].distance,
		// just to its left
		new Point(j, i - 1),
		    distances[j][i - 1].distance,
		// top-left corner
		new Point(j - 1, i - 1),
		    distances[j - 1][i - 1].distance +
			// add 1 if we need a replacement
			(text[j - 1] == query->query_str[i - 1] ? 0 : 1)
	    );
    }
}

// j is the index into the text,
// i is the index into the query
// assuming j != 0 and i != 0
int EditTable::set_distance(int j, int i) {
    Node &node = distances[j][i];

    // get the valid neighbour with the minimum edit distance
    node.from = min_neighbour(j, i);
    node.distance = dist(node.from);

    // if the two characters at this node are different,
    if(	query->query_str[i - 1] != text[j - 1] ) {
	// add 1 to the edit distance (replacement)
	node.distance++;
    }

    // return the edit distance of this node
    return node.distance;
}

// dump the whole table
void EditTable::dump() {
    // print the query characters
    cerr << '\t';
    for(int i = 0; i < query->query_len; i++) {
	cerr << '\t';
	pc(cerr, query->query_str[i]);
    }
    cerr << endl;

    for(int j = 0; j <= text_len; j++) {
	for(int i = 0; i <= query->query_len; i++) {
	    // print the text characters
	    if(i == 0 && j != 0) {
		pc(cerr, text[j - 1]);
	    }

	    cerr << '\t';

	    // print the actual distance
	    cerr << distances[j][i].distance;
	}
	cerr << endl;
    }
}

// traceback the best match
SearchResult *EditTable::get_match() {
    SearchResult *result;

    // if there is no match, return an empty search result
    if(best_match->j == INIT_POS) {
	return new SearchResult(0);
    }

    // if the match has too high a distance, return an empty search result
    if(dist(best_match) > query->errors_allowed) {
	return new SearchResult(0);
    }

    // allocate memory for the result
    result = new SearchResult(query->query_len + query->errors_allowed);

    // the easy stuff first
    result->errors = dist(best_match);
    result->match_len = 0;

    // start off at the node with the best match
    Point *current = best_match;
    while(current != NULL) {
	// add the next char to the result
	result->match[result->match_len++] = text[current->j - 1];

	// advance to the next node
	current = node(current).from;
    }

    // we don't really want to include the \0 char
    result->match_len--;

    // reverse the string in place
    strrev(result->match, result->match_len);

    /*// print the result
    if(__DEBUG__) {
	pc(cerr, result->match, result->match_len);
	cerr << endl;
    }*/

    return result;
}

void EditTable::free_rows() {
    // free the rows of the table
    for(int j = 0; j <= text_len; j++) {
	if(query != NULL) {
	    delete[] distances[j];
	}
    }
}

void EditTable::fill_table() {
    // there are going to be the same amount of creates/accesses here
    inc_comparisons((text_len + 1) * (query->query_len + 1));
    inc_ds((text_len + 1) * (query->query_len + 1));

    // reset the best match
    best_match = new Point(INIT_POS, INIT_POS);

    // the first row and column have a distance 0
    for(int j = 0; j <= text_len; j++) {
	// allocate memory for each row
	distances[j] = new Node[text_len + 1];

	// set the first column to 0
	distances[j][0].distance = 0;
    }
    for(int i = 0; i <= query->query_len; i++) {
	// set the first row to 0
	distances[0][i].distance = 0;
    }

    // first go through the text (rows)
    for(int j = 1; j <= text_len; j++) {
	// then go through the query (columns)
	for(int i = 1; i <= query->query_len; i++) {
	    
	    // work out the edit distance
	    set_distance(j, i);

	    // if we're in the last column (the end of the query string),
	    // [remember matches only happen at the end of the query string]
	    if(i == query->query_len) {
		// if this is the first time we get to the end of the query,
		// or if it's got a lower edit distance than the current
		// best match,
		if( best_match->j == INIT_POS ||
		    distances[j][i].distance < dist(best_match) ) {
		    // then update the best match to be this point
		    best_match->j = j;
		    best_match->i = i;
		}
	    }
	}
    }
}

SearchResult *EditTable::execute_query(SearchQuery *query) {
    SearchResult *result;

    // save the query
    this->query = query;

    // fill the table
    this->fill_table();
    
    // dump it
    if(__DEBUG__) {
	this->dump();
    }
    
    // get the best match
    result = get_match();

    // finally, return the match
    return result;
}

void EditTable::load_text(char *text, int len) {
    // save these values
    this->text = new char[len];
    strncpy(this->text, text, len);
    this->text_len = len;

    // build an empty array
    this->distances = new Node *[text_len + 1];
}

// used by main which doesn't know about the EditTable
Matcher *create_matcher() {
    return new EditTable();
}
