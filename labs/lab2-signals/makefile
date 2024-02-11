.PHONY: all clean

CC = clang
CFLAGS = -Og -g -std=c11 -Wall -pedantic -fsanitize=address,leak

LDFLAGS = -fsanitize=address,leak
LIBS = -lrt

all: chat

chat: signal-chat.o
	$(CC) $(CFLAGS) -o chat signal-chat.o $(LIBS) $(LDFLAGS)

signal-chat.o: signal-chat.c
	$(CC) $(CFLAGS) -c signal-chat.c 

clean:
	rm -rf chat signal-chat.o
