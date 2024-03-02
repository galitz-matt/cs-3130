#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


char *getoutput(const char *command) {
	int pipefd[2];
	pid_t cpid;
	char buffer[1024];
	int bytesRead;

	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) {
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		execl("/bin/sh", "sh", "-c", command, (char *) NULL);
		perror("execl");
		exit(EXIT_FAILURE);
	} else {
		close(pipefd[1]);
		char *output = malloc(1);
		output[0] = '\0';
		size_t totalSize = 1;
		
		while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
			buffer[bytesRead] = '\0';
			totalSize += bytesRead;
			output = realloc(output, totalSize);
			strcat(output, buffer);
		}
		close(pipefd[0]);
		waitpid(cpid, NULL, 0);
		return output;
	}
}

char *parallelgetoutput(int count, const char **argv_base) {
	int pipefd[2];
	if (pipe(pipefd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid_t *cpids = malloc(count * sizeof(pid_t));
	char *output = malloc(1);
	output[0] = '\0';
	size_t outputSize = 1;
	for (int i = 0; i < count; i++) {
		cpids[i] = fork();
		if (cpids[i] == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (cpids[i] == 0) {
			close(pipefd[0]);
			dup2(pipefd[1], STDOUT_FILENO);
			int argv_len;
			for (argv_len = 0; argv_base[argv_len] != NULL; argv_len++);
			char **args = malloc((argv_len + 2) * sizeof(char *));
			for (int j = 0; j < argv_len; j++) {
				args[j] = (char *)argv_base[j];
			}
			char indexStr[10];
			sprintf(indexStr, "%d", i);
			args[argv_len] = indexStr;
			args[argv_len + 1] = NULL;
			execv(argv_base[0], args);
			perror("execv");
			exit(EXIT_FAILURE);
		}
	}
	close(pipefd[1]);
	char buffer[256];
	ssize_t bytesRead;
	while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		size_t newLen = outputSize + bytesRead;
		output = realloc(output, newLen);
		strcat(output, buffer);
		outputSize = newLen;
	}

	for (int i = 0; i < count; i++) {
		waitpid(cpids[i], NULL, 0);
	}

	free(cpids);
	close(pipefd[0]);
	return output;
}

