Compiling
---------
to compile, use *make*. this will generate 3 executables:

- **trie**
- **edit**
- **mock** *(you can ignore this one)*

each of these files can be executed in 2 modes: **prompt** and **batch**.

- batch mode takes 3 arguments: *[text filename]* *[queries filename]* *[max errors]*.

it then executes all these queries in series and writes the output to stdout.

- prompt mode takes 1 argument: *[text filename]* - the file that contains the dictionary.

then it continually asks you for a query of the form *[max errors]* *[query string]*,
and gives you back the result and the instrumentation information straight away.

make sure that you use the script *preprocess.py* on the text file before giving
it to the trie or edit programs. to use that, just write "**python** *preprocess.py [filename]*"

Notes
-----

- the trie can't handle files input files that are too big, otherwise it runs out of memory.
  you may have to use a portion of the turgenev.txt file for this (up to the first ~400 words
  worked for me on my laptop)
- the edit distance program can't handle too many queries in a row, due to a memory leak
  I didn't have time to fix. so after around about 200 queries it crashes on my laptop.

Example use case
----------------

(assuming there's a file called turgenev.txt and a file called queries.txt)

**python** *preprocess.py turgenev.txt*

**make**

**./trie** *turgenev.txt queries.txt 1 > trie_with_1_error.out* # run the program in batch mode

**./trie** *turgenev.txt* # run the program in interactive mode