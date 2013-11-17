#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include "matcher.h"
#include "instrumentation.h"
#include "char.h"

#define PRINT_RESULTS true

using namespace std;

void print_usage_msg(char *);
void error_file(char *);
char *read_text_file(char *, int &);
SearchQuery *read_queries_file(char *, int &, int);
int num_lines(ifstream &);

void batch(Matcher *matcher, SearchQuery *queries, int size);
void prompt(Matcher *matcher);

int main(int argc, char **argv) {
    char *textdata;
    SearchQuery *queries;
    int size;
    Matcher *matcher;

    // make sure we have the correct number of arguments
    if(argc != 2 && argc != 4) {
	print_usage_msg(argv[0]);
	return 1;
    }

    // reset the instrumentation counts
    reset_counters();

    // create a new matcher
    matcher = create_matcher();

    // read the text data
    textdata = read_text_file(argv[1], size);

    // load it into the matcher
    matcher->load_text(textdata, size);

    // instrumentation data so far
    cout << size << " characters loaded." << endl;
    print_stats(matcher->size());
    cout << endl;

    // free up the text data
    delete textdata;

    // if we're in batch mode,
    if(argc == 4) {
	// read the query data
	queries = read_queries_file(argv[2], size, atoi(argv[3]));

	// execute them all
	batch(matcher, queries, size);

	// delete the queries
	delete[] queries;
    // if we're in prompt mode
    } else if(argc == 2) {
	// just start the prompt
	prompt(matcher);
    }

    // delete the matcher
    delete matcher;
}

void print_usage_msg(char *name) {
    cerr << "Usage:" << endl;
    cerr << name << " [text file] [queries file] [max number of errors]" << endl;
    cerr << "or" << endl;
    cerr << name << " [text file]" << endl;
    cerr << "Note that the files should not end with a new line char." << endl;
}

void error_file(char *filename) {
    cerr << "Error while opening file " << filename << endl;
    exit(1);
}

char *read_text_file(char *filename, int &size) {
    ifstream file;
    char *text;

    // open the file at the end
    file.open(filename, ios::ate);

    // if the file could not be opened, throw an error
    if(!file.is_open()) {
	error_file(filename);
    }

    // get the file size
    size = file.tellg();

    // allocate memory for the text
    text = new char[size];

    // read the text
    file.seekg(0, ios::beg);
    file.read(text, size);

    // close the file
    file.close();

    // return the text read
    return text;
}

SearchQuery *read_queries_file(char *filename, int &num_queries, int max_errors) {
    ifstream file;
    string temp;
    SearchQuery *queries;
    int current_query;

    // open the queries file
    file.open(filename);

    // if we can't open it, throw an error
    if(!file.is_open()) {
	error_file(filename);
    }

    // count the number of lines (= number of queries)
    num_queries = num_lines(file);

    // initialize the array of queries
    queries = new SearchQuery[num_queries];

    // go to the beginning of the file
    file.clear();
    file.seekg(0, ios::beg);

    // read them line by line
    for(current_query = 0; getline(file, temp); current_query++) {
	// initialize the array for the current query
	queries[current_query].set(temp.c_str(), temp.length(), max_errors);
    }

    // close the file
    file.close();

    // return the array
    return queries;
}

// stackoverflow.com/questions/3482064/counting-the-number-of-lines-in-a-text-file
int num_lines(ifstream &file) {
    // don't skip newlines
    file.unsetf(ios_base::skipws);

    // return the number of new line chars
    return count(
      istream_iterator<char>(file),
      istream_iterator<char>(),
      '\n'
    );
}

void batch(Matcher *matcher, SearchQuery *queries, int size) {
    SearchResult *result;

    // clear instrumentation data
    reset_counters();

    // for each query
    for(int i = 0; i < size; i++) {
	// execute the query and get the result
	result = matcher->execute_query(&(queries[i]));

	// print the result if needed
	if(PRINT_RESULTS) {
	    cout << "#" << i + 1 << ": {";
	    pc(cout, queries[i].query_str, queries[i].query_len);
	    cout << ", " << queries[i].query_len << ", " << queries[i].errors_allowed << "} -> ";
	    
	    if(result->has_value()) {
		cout << "{";
		pc(cout, result->match, result->match_len);
		cout << ", " << result->match_len << ", " << result->errors << "}";
	    } else {
		cout << "no matches";
	    }

	    cout << endl;
	}

	// free resources
	delete result;
    }

    // print total instrumentation data
    print_stats(matcher->size());

    cout << endl;
}

void prompt(Matcher *matcher) {
    SearchQuery *query;
    SearchResult *result;
    string input;

    // prompt the user to enter queries
    cout << "enter queries in the form <errors allowed> <query string>, or -1 to exit" << endl << endl;
    
    // keep executing queries
    while(true) {
	reset_counters();

	cout << "query> ";

	// create a query object for it
	query = new SearchQuery();

	cin >> query->errors_allowed;

	// detect if the user wants to exit
	if(query->errors_allowed == -1) {
	    break;
	}

	// take the input that the user gave
	getline(cin, input);

	// add it to the query structure
	query->query_str = new char[input.length() - 1];
	strncpy(query->query_str, input.c_str() + 1, input.length() - 1);
	query->query_len = input.length() - 1;

	// process the query
	result = matcher->execute_query(query);
	
	// if there was a match,
	if(result->has_value()) {
	    // print the result structure
	    cout << "{";
	    pc(cout, result->match, result->match_len);
	    cout << ", " << result->match_len << ", " << result->errors << "}" << endl;
	    
	    // print some instrumentation
	    print_stats(matcher->size());
	// otherwise,
	} else {
	    // show a disappointing message
	    cout << "no matches" << endl;
	}

	cout << endl;

	// free resources
	delete query;
	delete result;
    }
}
