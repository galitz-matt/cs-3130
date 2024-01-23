.PHONY: all clean

CC = clang

CFLAGS = -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak
LDFLAGS =
LIBS =

all: split.out

split.out: split.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o split.out split.o main.o $(LIBS)

split.o: split.c split.h
	$(CC) $(CFLAGS) -c split.c

main.o: main.c split.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o split.out
