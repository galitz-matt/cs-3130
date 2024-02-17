#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

pid_t other_pid = 0;
struct timespec start, end;
long reps = 10000;
long total_time;
long average_overhead_time;

void empty_function(void) __attribute__((noinline));
void empty_function(void) {}

void signal_handler(int sig) {
	if (sig == SIGUSR1) {
		kill(other_pid, SIGUSR2);
	} else if (sig == SIGUSR2) {
		clock_gettime(CLOCK_MONOTONIC, &end);
	}
}

long compute_ns(struct timespec start, struct timespec end) {
	return (end.tv_sec*1000000000 + end.tv_nsec) - (start.tv_sec*1000000000 + start.tv_nsec) - average_overhead_time;
}

void setup_signal_handling() {
	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = signal_handler; // Set the signal handler
	sigemptyset(&sa.sa_mask); // No signals blocked during handling
	sa.sa_flags = 0;

	// Setup for SIGUSR1
	if (sigaction(SIGUSR1, &sa, NULL) == -1) {
		perror("sigaction(SIGUSR1) failed");
		exit(EXIT_FAILURE);
	}

	// Setup for SIGUSR2
	if (sigaction(SIGUSR2, &sa, NULL) == -1) {
		perror("sigaction(SIGUSR2) failed");
		exit(EXIT_FAILURE);
	}
}

void wait_for_pid_and_signal() {
    printf("My PID: %d\n", getpid());
    printf("Enter the PID of the other process: ");
    scanf("%d", &other_pid);
    pause();
}

void measure_empty_function() {
	total_time = 0;
	for (int i = 0; i < reps; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		empty_function();
		clock_gettime(CLOCK_MONOTONIC, &end);
		total_time += compute_ns(start, end);
	}
	long average_time = (long) (total_time / reps);
	printf("AVG TIME 1: %ld ns\n", average_time);
}

void measure_getppid_call() {
	total_time = 0;
	for (int i = 0; i < reps; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		getppid();
		clock_gettime(CLOCK_MONOTONIC, &end);
		total_time += compute_ns(start, end);
	}
	long average_time = (long) (total_time / reps);
	printf("AVG TIME 2: %ld ns\n", average_time);
}

void measure_system_true() {
	total_time = 0;
	for (int i = 0; i < 10000; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		system("/bin/true");
		clock_gettime(CLOCK_MONOTONIC, &end);
		total_time += compute_ns(start, end);
	}
	long average_time = (long) (total_time / 10000);
	printf("AVG TIME 3: %ld ns\n", average_time);
}

void measure_signal_to_self() {
	setup_signal_handling();

	total_time = 0;
	for (int i = 0; i < reps; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		raise(SIGUSR2);
		total_time += compute_ns(start, end);
	}
	long average_time = (long) (total_time / reps);
	printf("AVG TIME 4: %ld ns\n", average_time);
}

void measure_inter_process_signal() {
	setup_signal_handling(); // Ensure signal handling is setup
    
	printf("This process PID: %d\n", getpid());
	printf("Enter the PID of the other process: ");
	scanf("%d", &other_pid);
	
	total_time = 0;
	for (int i = 0; i < reps; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		if (kill(other_pid, SIGUSR1) == -1) {
			perror("kill failed");
			exit(EXIT_FAILURE);
		} 
		pause();
		total_time += compute_ns(start, end);	
	}
	long average_time = (long) (total_time / reps);
	printf("AVG TIME 5: %ld ns\n", average_time);
}

int main(int argc, char *argv[]) {
	if (argc > 2) {
		fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
		return 1;
	}
	
	total_time = 0;
	for (int i = 0; i < reps; i++) {
		clock_gettime(CLOCK_MONOTONIC, &start);
		clock_gettime(CLOCK_MONOTONIC, &end);
		total_time += compute_ns(start, end);
	}
	average_overhead_time = (long) (total_time / (2 * reps));
	int mode = atoi(argv[1]);
	switch (mode) {
		case -1: // Special mode for waiting and responding to signals
			setup_signal_handling();
			printf("Special mode PID %d\n", getpid());
			printf("Enter the PID of the other process: ");
			scanf("%d", &other_pid);
			for (int i = 0 ; i < reps; i++) {
				pause();
			}
			break;
		case 1:
			measure_empty_function();
			break;
		case 2:
			measure_getppid_call();
			break;
		case 3:
			measure_system_true();
			break;
		case 4:
			measure_signal_to_self();
			break;
		case 5:
			measure_inter_process_signal();
			break;
		default:
			fprintf(stderr, "Invalid mode: %d\n", mode);
			return 1;
	}
	printf("Average overhead time: %ld ns\n", average_overhead_time);
	return 0;	
}

