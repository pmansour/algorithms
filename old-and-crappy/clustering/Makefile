# Makefile
# Project 2, COMP20003
# 2012s2

all: kmeans kmeans-naive kcenter single-linkage dbscan

kmeans: main.o smalloc.o point.o random.o kmeans.o
	$(CC) -o kmeans main.o smalloc.o point.o random.o kmeans.o

kmeans-naive: main.o smalloc.o point.o random.o kmeans-naive.o
	$(CC) -o kmeans-naive main.o smalloc.o point.o random.o kmeans-naive.o

kcenter: main.o common.o smalloc.o point.o random.o kcenter.o
	$(CC) -o kcenter common.o main.o smalloc.o point.o random.o kcenter.o

single-linkage: common.o cluster.o random.o main.o smalloc.o point.o single-linkage.o
	$(CC) -o single-linkage common.o cluster.o random.o main.o smalloc.o point.o single-linkage.o

dbscan: common.o main.o smalloc.o point.o dbscan.o
	$(CC) -o dbscan common.o main.o smalloc.o point.o dbscan.o

main.o: utils.h Makefile smalloc.h point.h clusterings.h
smalloc.o: Makefile smalloc.h
point.o: Makefile point.h utils.h
random.o: Makefile random.h
cluster.o: Makefile cluster.h smalloc.h
common.o: Makefile common.c
kmeans.o: Makefile random.h point.h utils.h clusterings.h
kmeans-naive.o: Makefile random.h point.h utils.h clusterings.h
kcenter.o: Makefile random.h point.h utils.h clusterings.h
single-linkage.o: Makefile common.h cluster.h random.h point.h utils.h clusterings.h
dbscan.o: Makefile common.h point.h utils.h clusterings.h

clean:
	\rm *.o kmeans kmeans-naive kcenter single-linkage dbscan

CC=gcc
CFLAGS= -std=c99 -O0 -g
