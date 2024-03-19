CC = clang
CFlAGS = -Wall -Wextra -std=c11
LDFLAGS =

LIB = libmlpt.a
OBJ = mlpt.o

all: $(LIB)

mlpt.o: mlpt.c mlpt.h
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB): $(OBJ)
	ar rcs $@ $^

clean:
	rm -f $(OBJ) $(LIB)

.PHONY: all clean
