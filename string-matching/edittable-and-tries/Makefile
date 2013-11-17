OBJ = main.o instrumentation.o char.o trie.o edit.o mock_matcher.o
BIN = mock trie edit
CFLAGS = -c
CC = g++

# real builds

all: edit trie mock

mock: main.o instrumentation.o char.o mock_matcher.o
	$(CC) main.o instrumentation.o char.o mock_matcher.o -o mock

trie: main.o instrumentation.o char.o trie.o
	$(CC) main.o instrumentation.o char.o trie.o -o trie

edit: main.o instrumentation.o char.o edit.o
	$(CC) main.o instrumentation.o char.o edit.o -o edit

# object files

mock_matcher.o: mock_matcher.cpp matcher.h
	$(CC) $(CFLAGS) mock_matcher.cpp

edit.o: edit.cpp instrumentation.h matcher.h edit.h
	$(CC) $(CFLAGS) edit.cpp

trie.o: trie.cpp instrumentation.h matcher.h trie.h
	$(CC) $(CFLAGS) trie.cpp

char.o: char.cpp char.h
	$(CC) $(CFLAGS) char.cpp

instrumentation.o: instrumentation.cpp instrumentation.h
	$(CC) $(CFLAGS) instrumentation.cpp

main.o: main.cpp instrumentation.h matcher.h
	$(CC) $(CFLAGS) main.cpp

# pseudo-targets

clean:
	rm -f $(OBJ)

realclean: clean
	rm -f $(BIN)
