#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barrier;
pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void random_sleep(int attempt) {
    // Random backoff to prevent livelock, increases with the number of attempts
    unsigned int backoff_time = (rand() % 50 + 50) * (attempt + 1); // Exponential backoff strategy
    usleep(backoff_time); // Sleep for backoff_time microseconds
}

void *eat(void *arg) {
    int n = (int)(long)arg;
    int leftChopstick = n;
    int rightChopstick = (n + 1) % 5;
    int lockStatus, attempts = 0;

    pthread_barrier_wait(&barrier);

    while (1) {
        pthread_mutex_lock(&chopstick[leftChopstick]);
        printf("Philosopher %d picked up chopstick %d\n", n, leftChopstick);

        lockStatus = pthread_mutex_trylock(&chopstick[rightChopstick]);
        if (lockStatus == 0) { // Successfully acquired second chopstick
            printf("Philosopher %d picked up chopstick %d\n", n, rightChopstick);
            printf("Philosopher %d is eating\n", n);
            sleep(1); // Simulate eating
            
            pthread_mutex_unlock(&chopstick[rightChopstick]);
            pthread_mutex_unlock(&chopstick[leftChopstick]);
            break; // Finished eating, exit loop
        } else {
            // Failed to acquire second chopstick, release the first and retry after backoff
            pthread_mutex_unlock(&chopstick[leftChopstick]);
            attempts++;
            random_sleep(attempts); // Random backoff to prevent livelock
        }
    }

    return NULL;
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    pthread_barrier_init(&barrier, NULL, 5);

    for (int i = 0; i < 5; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for (int i = 0; i < 5; i++)
        pthread_create(&philosopher[i], NULL, eat, (void*)(size_t)i);

    for (int i = 0; i < 5; i++)
        pthread_join(philosopher[i], NULL);

    for (int i = 0; i < 5; i++)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}

