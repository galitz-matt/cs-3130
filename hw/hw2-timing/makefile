.PHONY: all, clean

CC=clang
CFLAGS=-O1 -Wall -std=c99
LFLAGS=-lrt

all: gettimings

gettimings: gettimings.c
	$(CC) $(CFLAGS) gettimings.c -o gettimings $(LFLAGS)

clean:
	rm -f gettimings
