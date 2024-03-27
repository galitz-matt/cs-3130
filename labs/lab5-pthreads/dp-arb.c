#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barrier;
pthread_t philosopher[5];
pthread_mutex_t chopstick[5];
pthread_mutex_t arbitrator; // Global arbitrator mutex

void *eat(void *arg) {
    int n = (int) (long)arg;

    pthread_barrier_wait(&barrier);

    // Lock the arbitrator mutex before attempting to pick up any chopsticks
    pthread_mutex_lock(&arbitrator);

    pthread_mutex_lock(&chopstick[n]);
    printf("Philosopher %d got chopstick %d\n", n, n);
    pthread_mutex_lock(&chopstick[(n+1)%5]);
    printf("Philosopher %d got chopstick %d\n", n, (n+1)%5);
    
    printf ("Philosopher %d is eating\n",n);
    sleep(1);

    pthread_mutex_unlock(&chopstick[(n+1)%5]);
    printf("Philosopher %d set down chopstick %d\n", n, (n+1)%5);
    pthread_mutex_unlock(&chopstick[n]);
    printf("Philosopher %d set down chopstick %d\n", n, n);

    // Unlock the arbitrator mutex after putting down the chopsticks
    pthread_mutex_unlock(&arbitrator);

    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < 5; i += 1)
        pthread_mutex_init(&chopstick[i], NULL);

    // Initialize the arbitrator mutex
    pthread_mutex_init(&arbitrator, NULL);

    for(int i =0; i < 5; i += 1)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i=0; i < 5; i += 1)
        pthread_join(philosopher[i], NULL);

    for(int i=0; i < 5; i += 1)
        pthread_mutex_destroy(&chopstick[i]);

    // Destroy the arbitrator mutex
    pthread_mutex_destroy(&arbitrator);

    pthread_barrier_destroy(&barrier);

    return 0;
}

