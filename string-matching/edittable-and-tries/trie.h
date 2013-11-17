#ifndef __TRIE_H_INCLUDED__
#define __TRIE_H_INCLUDED__

#include <cstring>
#include "instrumentation.h"
#include "matcher.h"

// a Trie that holds our text
class Trie : public Matcher {
    char key;
    char *direct;

    // for traceback
    Trie *parent;
    int depth;
    char *traceback;

    // the tree bits
    Trie *children;
    Trie *next;

    int num_children;

    // append a new child with the given key to the front of children
    void append_child(char key, char *pointer);
    // return a child with the given key, or null if there isn't one
    Trie *get_child(char key);
    // dump the contents of the trie with text starting at the given pointer
    void dump(char *text);
 
    char *find_traceback();

    public:

    // constructor: starts off as a direct pointer
    Trie(char key, char *pointer, Trie *parent, int depth);

    // destructor : free the children
    ~Trie();

    // add the suffix starting at the given pointer into a null-terminated
    // string, while comparing the char at key + offset.
    // when called from outside, offset should be 0
    void add_suffix(char *key, int offset);

    // search for the string in query starting from index, keeping in
    // mind the errors so far. put the result in result
    void approx_match(SearchQuery *query, SearchResult *result, int index, int errors_sofar);

    /*// add the (not null-terminated) string starting at the given pointer
    // with the given length.
    void add_string(char *string, int len);

    // search for the given query, with the given path
    void search(SearchQuery &query, SearchState state);*/

    // some matcher functions
    void load_text(char *text, int size);
    SearchResult *execute_query(SearchQuery *query);
    size_t size() { return sizeof(Trie); }
};

// the size of 1 Data Structure is the size of a trie
size_t ds_size() { return sizeof(Trie); }

#endif
