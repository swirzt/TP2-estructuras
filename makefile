CC=gcc
FLAGS=-std=c99 -pedantic -Wall

all: btree.o tablahash.o main

btree.o: btree.h btree.c
	$(CC) -c btree.c $(FLAGS)

tablahash.o: tablahash.c tablahash.h
	$(CC) -c tablahash.c $(FLAGS)

colasenlazadas.o: colasenlazadas.c colasenlazadas.h
	$(CC) -c colasenlazadas.c $(FLAGS)

main: tablahash.o main.c btree.o colasenlazadas.o
	$(CC) -o main main.c tablahash.o btree.o colasenlazadas.o $(FLAGS)