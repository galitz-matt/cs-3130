#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <unistd.h>

pthread_barrier_t barrier;
pthread_t philosopher[5];
pthread_mutex_t chopstick[5];

void *eat(void *arg) {
    int n = (int) (long)arg;
    int firstChopstick = n;
    int secondChopstick = (n + 1) % 5;

    if (firstChopstick > secondChopstick) {
        int temp = firstChopstick;
        firstChopstick = secondChopstick;
        secondChopstick = temp;
    }

    pthread_barrier_wait(&barrier);

    pthread_mutex_lock(&chopstick[firstChopstick]);
    printf("Philosopher %d got chopstick %d\n", n, firstChopstick);
    pthread_mutex_lock(&chopstick[secondChopstick]);
    printf("Philosopher %d got chopstick %d\n", n, secondChopstick);

    printf ("Philosopher %d is eating\n", n);
    sleep(1); // Simulate eating time

    pthread_mutex_unlock(&chopstick[secondChopstick]);
    pthread_mutex_unlock(&chopstick[firstChopstick]);

    return NULL;
}

int main(int argc, const char *argv[]) {
    pthread_barrier_init(&barrier, NULL, 5);

    for(int i = 0; i < 5; i++)
        pthread_mutex_init(&chopstick[i], NULL);

    for(int i = 0; i < 5; i++)
        pthread_create(&philosopher[i], NULL, eat, (void *)(size_t)i);

    for(int i = 0; i < 5; i++)
        pthread_join(philosopher[i], NULL);

    for(int i = 0; i < 5; i++)
        pthread_mutex_destroy(&chopstick[i]);

    pthread_barrier_destroy(&barrier);

    return 0;
}

