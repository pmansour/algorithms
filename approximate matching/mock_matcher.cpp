#include <cstring>
#include <iostream>
#include "matcher.h"

using namespace std;

class MockMatcher : public Matcher {

    public:
	void load_text(char *text, int size);
	SearchResult *execute_query(SearchQuery *query);
	size_t size() { return sizeof(MockMatcher); }
};

void MockMatcher::load_text(char *text, int size) {
    // note that this method has been called
    cerr << "loaded text of size " << size << endl;
}

SearchResult *MockMatcher::execute_query(SearchQuery *query) {
    // note that this method has been called
    cerr << "executed \"" << query->query_str << "\" (" << query->errors_allowed << ")" << endl;
    // return some random result
    SearchResult *result = new SearchResult(query->query_len);

    strncpy(result->match, query->query_str, query->query_len);

    result->match_len = query->query_len;
    result->errors = 0;

    return result;
}

Matcher *create_matcher() {
    return new MockMatcher;
}
