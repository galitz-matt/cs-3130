#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#include "my_system.h"

int my_system(const char *command) {
	int status;
	pid_t pid = fork();
	if (pid == -1) {
		perror("Could not create child process");
		exit(EXIT_FAILURE);
	} else if (pid > 0) {
		waitpid(pid, &status, 0);
		if (WIFEXITED(status)) {
			return WIFEXITED(status);
		} else {
			return -1;
		}
	} else {
		execl("/bin/sh", "sh", "-c", command, (char*)NULL);
		perror("excel returned");
		exit(EXIT_FAILURE);
	}
}
