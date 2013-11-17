CC = g++
CFLAGS = -std=c++11

BWT: burrows-wheeler.o
	$(CC) -o bwt burrows-wheeler.cpp

burrows-wheeler.o: burrows-wheeler.cpp burrows-wheeler.h
	$(CC) $(CFLAGS) -c burrows-wheeler.cpp
