.PHONY: all clean

CC = clang
CFLAGS = -D_XOPEN_SOURCE=700 -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak

LDFLAGS = -fsanitize=address,leak
LIBS =

%.o: %.c say.h
	$(CC) $(CFLAGS) -c $< -o $@

all: guesser

guesser: libsay.a guesser.o
	$(CC) $(LDFLAGS) -o guesser guesser.o -L. -lsay $(LIBS)

libsay.a: cheer.o grunt.o
	ar -rcs libsay.a $^

clean:
	rm -f *.o guesser libsay.a
