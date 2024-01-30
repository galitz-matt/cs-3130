.PHONY: all clean

CC = clang

CFLAGS = -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak
LDFLAGS =
LIBS =

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: split.out

split.out: split.o main.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o split.out $^ $(LIBS)

clean:
	rm -f *.o split.out
