CC=gcc
FLAGS= -std=c99 -pedantic -Wall

all: btree.o tablahash.o slist.o colasenlazadas.o main

btree.o: btree.h btree.c
	$(CC) -c btree.c $(FLAGS)

tablahash.o: tablahash.c tablahash.h btree.o
	$(CC) -c tablahash.c $(FLAGS)

slist.o: slist.c slist.h
	$(CC) -c slist.c $(FLAGS)

colasenlazadas.o: colasenlazadas.c colasenlazadas.h slist.o
	$(CC) -c colasenlazadas.c $(FLAGS)

main: tablahash.o main.c btree.o slist.o colasenlazadas.o
	$(CC) -o main main.c tablahash.o btree.o slist.o colasenlazadas.o $(FLAGS)

clean:
	rm -rf btree.o
	rm -rf tablahash.o
	rm -rf slist.o
	rm -rf colasenlazadas.o
	rm -rf main