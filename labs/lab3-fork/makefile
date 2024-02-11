.PHONY: all clean

CC = clang
CFLAGS = -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak

all: main

main: main.o my_system.o
	$(CC) $(CFLAGS) -o main main.o my_system.o

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

my_system.o: my_system.c my_system.h
	$(CC) $(CFLAGS) -c -o my_system.o my_system.c

clean:
	rm -rf main
