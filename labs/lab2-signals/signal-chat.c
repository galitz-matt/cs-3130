#define _XOPEN_SOURCE 700 
#include <stdlib.h>      
#include <stdio.h>      
#include <unistd.h>    
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>


pid_t other_pid = 0;
int inbox_fd = -1;
int outbox_fd = -1;
char *inbox_data = NULL;
char *outbox_data = NULL;
char inbox_filename[512];
char outbox_filename[512];

void cleanup() {
	if (inbox_data != MAP_FAILED) {
		munmap(inbox_data, 4096);
		inbox_data = NULL;
	}
	if (outbox_data != MAP_FAILED) {
		munmap(outbox_data, 4096);
		outbox_data = NULL;
	}
	if (inbox_fd != -1) {
		close(inbox_fd);
		shm_unlink(inbox_filename);
	}
	if (outbox_fd != -1) {
		close(outbox_fd);
	}
}

static void sigint_handler(int signum) {
	cleanup();
	if (other_pid > 0) {
		kill(other_pid, SIGTERM);	
	}
	exit(EXIT_SUCCESS);
}

static void sigterm_handler(int signum) {
	cleanup();
	exit(EXIT_SUCCESS);
}

static void sigusr1_handler(int signum) {
	fputs(inbox_data, stdout);
	fflush(stdout);
	inbox_data[0] = '\0';
}


int main() {
	// get PIDs
	printf("This process's ID: %ld\n", (long) getpid());
	char *line = NULL;
	size_t line_length = 0;
	do {
		printf("Enter other process ID: ");
		if (-1 == getline(&line, &line_length, stdin)) {
			perror("getline");
			abort();
		}
	} while ((other_pid = strtol(line, NULL, 10)) == 0);
	free(line);
	
	// intialize signal handlers
	struct sigaction sa_int;
	struct sigaction sa_term;
	struct sigaction sa_usr1;

	sa_int.sa_handler = sigint_handler;
	sa_term.sa_handler = sigterm_handler;
	sa_usr1.sa_handler = sigusr1_handler;
	
	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_term.sa_mask);
	sigemptyset(&sa_usr1.sa_mask);

	sa_int.sa_flags = SA_RESTART;
	sa_term.sa_flags = SA_RESTART;
	sa_usr1.sa_flags = SA_RESTART;

	if (sigaction(SIGINT, &sa_int, NULL) == -1) {
		perror("unable to override ^C signal");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGTERM, &sa_term, NULL) == -1) {
		perror("unable to override termination signal");
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
		perror("unable to override usr1 signal");
		exit(EXIT_FAILURE);
	}
	
	// create shared memory
	snprintf(inbox_filename, sizeof(inbox_filename), "/%d-chat", getpid());
	snprintf(outbox_filename, sizeof(outbox_filename), "%d-chat", other_pid);
	inbox_fd = shm_open(inbox_filename, O_CREAT | O_RDWR, 0666);
	outbox_fd = shm_open(outbox_filename, O_CREAT | O_RDWR, 0666);
	if (inbox_fd < 0 || outbox_fd < 0) {
		perror("shm_open");
		exit(EXIT_FAILURE);	
	}
	// truncate files to 4096 bytes
	if (ftruncate(inbox_fd, 4096) == -1 || ftruncate(outbox_fd, 4096) == -1) {
		perror("ftruncate");
		close(inbox_fd);
		close(outbox_fd);
		exit(EXIT_FAILURE);
	}
	
	inbox_data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, inbox_fd, 0);
	outbox_data = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, outbox_fd, 0);
	if (inbox_data == (char*) MAP_FAILED || outbox_data == (char*) MAP_FAILED) {
		perror("mmap");
		exit(EXIT_FAILURE);
	}
	close(inbox_fd);
	close(outbox_fd);
	
	while(fgets(outbox_data, 4096, stdin) != NULL) {
		kill(other_pid, SIGUSR1);
		while (outbox_data[0] != '\0') {
			usleep(10000);
		}
	}
	kill(other_pid, SIGTERM);
}
