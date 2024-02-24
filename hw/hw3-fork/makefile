CC = clang
CFLAGS = -fsanitize=address -ftrivial-auto-var-init=pattern -Og -g

fork_run: fork_run.c
	$(CC) $(CFLAGS) -o fork_run fork_run.c

clean:
	rm -f fork_run

.PHONY: clean
