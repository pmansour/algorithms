#include "instrumentation.h"
#include "char.h"
#include "trie.h"
#include <cstdlib>
#include <iostream>

#define __DEBUG__ false

using namespace std;

// constructor
Trie::Trie(char key, char *pointer, Trie *parent, int depth) {
    // most importantly: the key
    this->key = key;

    // every node starts off as a direct pointer (leaf)
    this->direct = pointer;

    // traceback stuff
    this->parent = parent;
    this->depth = depth;
    this->traceback = this->find_traceback();

    // tree stuff
    this->next = NULL;
    this->children = NULL;

    this->num_children = 0;

    // record a new data structure being created
    inc_ds();

    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "()";
	cerr << endl;
    }
}

// destructor
Trie::~Trie() {
    // free the children
    for(Trie *doomed = children; doomed != NULL; doomed = children) {
	children = doomed->next;
	delete doomed;
    }
}

// add a new child Trie with the given key
void Trie::append_child(char key, char *pointer) {
    Trie *new_child;

    // create a new Trie with the given key
    new_child = new Trie(key, pointer, this, this->depth + 1);

    // add it to the front of this Trie's children
    new_child->next = this->children;
    this->children = new_child;

    // increment the number of children so far
    this->num_children++;

    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "::append_child\t";
	pc(cerr, key);
	cerr << endl << endl;
    }
}

Trie *Trie::get_child(char key) {
    Trie *next_child;

    // go through all the Trie's children looking for this key
    for(    next_child = this->children;
	    next_child != NULL;
	    next_child = next_child->next   ) {

	// record a comparison being made
	inc_comparisons();

	// if this is the child we're looking for, just return it
	if(next_child->key == key) {
	    return next_child;
	}

    }

    // since we haven't found one, return null
    return NULL;
}

// do a breadth-first search
void Trie::dump(char *text) {
    // indent based on its depth
    cout << this->depth << "-";
    for(int i = 0; i < this->depth; i++) {
	cout << " ";
    }
    // print itself
    pc(cout, this->traceback, this->depth);
    
    // print its number of children
    cout << " (" << this->num_children << " children)";
    // print its pointer index
    if(this->num_children == 0 && this->direct != NULL) {
	cout << " -> " << (this->direct - text);
    }

    cout << endl;

    // print its children
    for(Trie *child = this->children; child != NULL; child = child->next) {
	child->dump(text);
    }
}

char *Trie::find_traceback() {
    Trie *current;
    char *path;
    
    // start off from this trie
    current = this;
 
    // allocate memory for the path (account for \0 at the end)
    path = new char[this->depth + 1];

    // add the null-terminator char
    path[this->depth] = '\0';

    // going backwards,
    inc_comparisons(this->depth);
    for(int i = this->depth - 1; i >= 0; i--) {
	// add the key of the current Trie to the path
	path[i] = current->key;
	// move back
	current = current->parent;
    }

    // return the path we found
    return path;
}

// offset is initially 0
// assumes text that key is pointing to ends with \0
void Trie::add_suffix(char *key, int offset) {
    char looking_for;
    Trie *child;
    Trie *to_return;

    // we're looking to compare the char at key + offset
    looking_for = *(key + offset);

    // look for it in this Trie's children
    child = this->get_child(looking_for);

    // if it doesn't exist, add it
    inc_comparisons();
    if(child == NULL) {
	// add it, with the given key char and pointer
	append_child(looking_for, key);

	// look for it again now that we've added it
	child = this->get_child(looking_for);
    }

    // if we're not at the end of the string
    inc_comparisons();
    if(looking_for != '\0') {
	// add the next char in the suffix to the child
	child->add_suffix(key, offset + 1);
    }
}

void Trie::load_text(char *text, int size) {
    // for each suffix
    inc_comparisons(size);
    for(int i = 0; i < size; i++) {
	// add it to the root trie
	this->add_suffix(text + i, 0);

	// debug message
	if(__DEBUG__) {
	    pc(cerr, this->traceback, this->depth);
	    cerr << "::load_text\tadded suffix starting at ";
	    pc(cerr, *(text + i));
	    cerr << " to root" << endl;
	    cerr << endl << "--------------------------" << endl << endl;
	}
    }

    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "::load_text\tloaded text of size " << size << endl << endl;
	// dump it
	this->dump(text);
	cout << endl;
    }
}

void Trie::approx_match(SearchQuery *query, SearchResult *result, int index, int errors_sofar) {
    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "::approx_match\t";
	cerr << "query = {";
	pc(cerr, query->query_str, query->query_len);
	cerr << ", " << query->query_len << ", " << query->errors_allowed << "}, ";
	
	if(result->match_len == -1) {
	    cerr << "result = empty, ";
	} else {
	    cerr << "result = {";
	    pc(cerr, result->match, result->match_len);
	    cerr << ", " << result->match_len << ", " << result->errors << "}, ";
	}

	cerr << "index = " << index << ", ";
	cerr << "errors_sofar = " << errors_sofar;
	cerr << " )" << endl;
    }

    // if there are too many errors, exit
    inc_comparisons();
    if(errors_sofar > query->errors_allowed) {
	return;
    }

    // if the query is done,
    inc_comparisons();
    if(index == query->query_len) {
	// debug message
	if(__DEBUG__) {
	    cerr << "success!";
	}

	// try to set the result to this path
	result->try_update(this->traceback, this->depth, errors_sofar);

	// debug message
	if(__DEBUG__) {
	    cerr << " result = {";
	    pc(cerr, result->match, result->match_len);
	    cerr << ", " << result->match_len << ", " << result->errors << "}" << endl;
	}

	// exit
	return;
    }

    // otherwise

    // try a deletion (the same node but at the next char)
    this->approx_match(query, result, index + 1, errors_sofar + 1);

    // for each child,
    for(Trie *child = this->children; child != NULL; child = child->next) {
	// there are 2 comparisons here
	inc_comparisons(2);

	// if it's an exact match,
	if(child->key == query->query_str[index]) {
	    // go down that path exactly
	    child->approx_match(query, result, index + 1, errors_sofar);
	    // we don't need to worry about approx matches for this node
	    return;
	}

	// otherwise,
	
	// try a replacement
	child->approx_match(query, result, index + 1, errors_sofar + 1);

	// try an insertion (the same char but at the next node)
	child->approx_match(query, result, index, errors_sofar + 1);
    }

    // try a deletion (the same node but at the next char)
    this->approx_match(query, result, index + 1, errors_sofar + 1);
}

SearchResult *Trie::execute_query(SearchQuery *query) {
    SearchResult *result;

    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "::execute_query\t";
	cerr << "starting search for ";
	pc(cerr, query->query_str, query->query_len);
	cerr << endl;
    }

    // create an empty result that can fit the biggest possible match
    result = new SearchResult(query->query_len + query->errors_allowed);

    // search for the given query
    this->approx_match(query, result, 0, 0);

    // debug message
    if(__DEBUG__) {
	pc(cerr, this->traceback, this->depth);
	cerr << "::execute_query\tfinished searching for ";
	pc(cerr, query->query_str, query->query_len);
	cerr << endl << endl;
    }

    // return the result
    return result;
}

// for the "interface"
Matcher *create_matcher() {
    return new Trie('.', NULL, NULL, 0);
}
