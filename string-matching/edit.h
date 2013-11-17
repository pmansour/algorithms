#ifndef __EDIT_H_INCLUDED__
#define __EDIT_H_INCLUDED__
#include "matcher.h"

// represents a point in our table
struct Point {
    // the column - corresponding to the char in the text
    int j;
    // the row - corresponding to the char in the query
    int i;

    Point(int j, int i) {
	this->j = j;
	this->i = i;
    }
};

// a node in the table
struct Node {
    // the edit distance
    int distance;
    // the point it came from
    Point *from;

    // deleting it should also delete the point
    ~Node() {
	delete from;
    }
};

class EditTable : public Matcher {
    // the actual table
    Node **distances;
    char *text;
    int text_len;
    SearchQuery *query;
    // the node with the lowest edit distance in the last column
    Point *best_match;

    void dump();
    Point *min_neighbour(int j, int i);
    // returns the distance that was set
    int set_distance(int j, int i);
    void fill_table();
    void free_rows();
    SearchResult *get_match();

    public:

    EditTable();
    ~EditTable();

    // overriden high-level methods
    void load_text(char *text, int size);
    SearchResult *execute_query(SearchQuery *query);
    size_t size() { return sizeof(Node) + sizeof(Point); }
};

#endif
