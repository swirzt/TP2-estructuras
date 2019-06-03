CC=gcc
FLAGS=-g -std=c99 -pedantic -Wall

all: btree.o tablahash.o colasenlazadas.o main

btree.o: btree.h btree.c
	$(CC) -c btree.c $(FLAGS)

tablahash.o: tablahash.c tablahash.h
	$(CC) -c tablahash.c $(FLAGS)

slist.o: slist.c slist.h
	$(CC) -c slist.c $(FLAGS)

colasenlazadas.o: colasenlazadas.c colasenlazadas.h slist.o
	$(CC) -c colasenlazadas.c $(FLAGS)

main: tablahash.o main.c btree.o slist.o colasenlazadas.o
	$(CC) -o main main.c tablahash.o btree.o slist.o colasenlazadas.o $(FLAGS)