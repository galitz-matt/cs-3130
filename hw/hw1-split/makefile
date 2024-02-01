.PHONY: all clean

CC = clang

CFLAGS = -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak
LDFLAGS =
LIBS =

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: split

split: split.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o split $^ $(LIBS)

clean:
	rm -f *.o split
