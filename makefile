CC=gcc
FLAGS=-std=c99 -pedantic -Wall

all: btree.o tablahash.o main

btree.o: btree.h btree.c
	$(CC) -c btree.c $(FLAGS)

tablahash.o: tablahash.c tablahash.h
	$(CC) -c tablahash.c $(FLAGS)

main: tablahash.o main.c btree.o
	$(CC) -o main main.c tablahash.o btree.o $(FLAGS)