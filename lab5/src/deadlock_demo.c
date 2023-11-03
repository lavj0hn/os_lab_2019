#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg) {
    pthread_mutex_lock(&mutex1);
    printf("Thread 1: mutex1 locked\n");
    // Даем возможность потоку 2 запуститься и заблокировать mutex2
    sleep(1);
    printf("Thread 1: trying to lock mutex2\n");
    pthread_mutex_lock(&mutex2);
    printf("Thread 1: mutex2 locked\n");
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    return NULL;
}

void *thread2(void *arg) {
    pthread_mutex_lock(&mutex2);
    printf("Thread 2: mutex2 locked\n");
    // Даем возможность потоку 1 запуститься и заблокировать mutex1
    sleep(1);
    printf("Thread 2: trying to lock mutex1\n");
    pthread_mutex_lock(&mutex1);
    printf("Thread 2: mutex1 locked\n");
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    if (pthread_create(&t1, NULL, thread1, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }
    if (pthread_create(&t2, NULL, thread2, NULL) != 0) {
        perror("pthread_create");
        return 1;
    }

    if (pthread_join(t1, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }
    if (pthread_join(t2, NULL) != 0) {
        perror("pthread_join");
        return 1;
    }

    printf("Finished\n");
    return 0;
}