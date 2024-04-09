#include "life.h"
#include <pthread.h>

typedef struct {
    int start, end, steps;
    LifeBoard *state;
    LifeBoard *next_state;
    pthread_barrier_t *my_barrier;
} ThreadInfo;

void *util(void *argument) {
    ThreadInfo *my_info = (ThreadInfo*) argument;

    for (int step = 0; step < my_info->steps; step++) {
        for (int y = my_info->start; y < my_info->end; y++) {
            for (int x = 1; x < my_info->state->width - 1; x++) {
                int live_in_window = 0;
                for (int y_offset = -1; y_offset <= 1; y_offset++)
                    for (int x_offset = -1; x_offset <= 1; x_offset++)
                        if (LB_get(my_info->state, x + x_offset, y + y_offset))
                            live_in_window++;

                LB_set(my_info->next_state, x, y,
                       live_in_window == 3 ||
                       (live_in_window == 4 && LB_get(my_info->state, x, y)));
            }
        }
        pthread_barrier_wait(my_info->my_barrier);
        pthread_barrier_wait(my_info->my_barrier);
    }

    return NULL;
}

void simulate_life_parallel(int threads, LifeBoard *state, int steps) {
    pthread_barrier_t barrier;
    pthread_t pthreads[threads];
    ThreadInfo info[threads];

    pthread_barrier_init(&barrier, NULL, threads + 1);
    LifeBoard *new_state = LB_new(state->width, state->height);
    int factor = state->height / threads;

    for (int i = 0; i < threads; i++) {
        info[i].start = (i == 0) ? 1 : i * factor;
        info[i].end = (i == threads - 1) ? state->height - 1 : (i + 1) * factor;
        info[i].state = state;
        info[i].next_state = new_state;
        info[i].steps = steps;
        info[i].my_barrier = &barrier;
        pthread_create(&pthreads[i], NULL, util, &info[i]);
    }

    for (int step = 0; step < steps; step++) {
        pthread_barrier_wait(&barrier);
        LB_swap(state, new_state);
        pthread_barrier_wait(&barrier);
    }

    for (int i = 0; i < threads; i++) {
        pthread_join(pthreads[i], NULL);
    }

    pthread_barrier_destroy(&barrier);
    LB_del(new_state);
}
