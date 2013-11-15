# Makefile for basic data structures
# awirth
# aug 09, jul 12
# comp20003


all: testa testoa testl testb testrbt generate gen_all test_all

test_all: test_all.o Makefile smalloc.o time.o
	$(CC) $(CFLAGS) -o test_all test_all.o smalloc.o time.o

gen_all: gen_all.o Makefile
	$(CC) $(CFLAGS) -o gen_all gen_all.o

generate: generate.o Makefile smalloc.o
	$(CC) $(CFLAGS) -o generate generate.o smalloc.o

testa: main.o array.o Makefile smalloc.o record.o report.o
	$(CC) $(CFLAGS) -o testa main.o array.o smalloc.o record.o report.o

testoa: main.o orderedarray.o Makefile smalloc.o record.o report.o
	$(CC) $(CFLAGS) -o testoa main.o orderedarray.o smalloc.o record.o report.o

testl: main.o linked.o Makefile smalloc.o record.o report.o
	$(CC) $(CFLAGS) -o testl main.o linked.o smalloc.o record.o report.o

testb: main.o linked.o Makefile smalloc.o record.o bst.o report.o
	$(CC) $(CFLAGS) -o testb main.o bst.o smalloc.o record.o report.o

testrbt: main.o rbt.o Makefile smalloc.o record.o report.o
	$(CC) $(CFLAGS) -o testrbt main.o rbt.o smalloc.o record.o report.o

array.c: smalloc.h record.h utils.h dict.h Makefile report.h
orderedarray.c: smalloc.h record.h utils.h dict.h Makefile report.h
linked.c: smalloc.h record.h utils.h dict.h Makefile report.h
bst.c: smalloc.h record.h utils.h dict.h Makefile report.h
rbt.c: smalloc.h record.h utils.h dict.h Makefile report.h
report.c: utils.h report.h
test_all.c: time.h
time.c: time.h

main.c: utils.h dict.h Makefile

clean:
	/bin/rm -f $(OBJ) testoa testa testl testb testrbt generate gen_all test_all

OBJ = time.o report.o record.o array.o orderedarray.o smalloc.o main.o linked.o bst.o rbt.o generate.o gen_all.o test_all.o
CC=gcc
CFLAGS= -std=c99 -g -O0 # useful for debugging
