#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <getopt.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long long result = 1;

struct ThreadArgs {
    int start;
    int end;
    int mod;
};

void *factorial(void *args) {
    struct ThreadArgs *arguments = (struct ThreadArgs *)args;
    long long local_result = 1;
    for (int i = arguments->start; i <= arguments->end; i++) {
        local_result = (local_result * i) % arguments->mod;
    }

    pthread_mutex_lock(&mutex);
    result = (result * local_result) % arguments->mod;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int k = -1, pnum = -1, mod = -1;
    int option;
    while ((option = getopt(argc, argv, "k:p:m:")) != -1) {
        switch (option) {
            case 'k':
                k = atoi(optarg);
                break;
            case 'p':
                pnum = atoi(optarg);
                break;
            case 'm':
                mod = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -k <number> -p <threads> -m <mod>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (k == -1 || pnum == -1 || mod == -1) {
        fprintf(stderr, "Usage: %s -k <number> -p <threads> -m <mod>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pthread_t threads[pnum];
    struct ThreadArgs args[pnum];

    int chunk_size = k / pnum;
    int remainder = k % pnum;

    int start = 1;
    for (int i = 0; i < pnum; i++) {
        args[i].start = start;
        args[i].end = start + chunk_size - 1;
        if (remainder > 0) {
            args[i].end++;
            remainder--;
        }
        args[i].mod = mod;
        start = args[i].end + 1;

        if (pthread_create(&threads[i], NULL, factorial, (void *)&args[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < pnum; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    printf("Factorial of %d modulo %d is: %lld\n", k, mod, result);
    pthread_mutex_destroy(&mutex);
    return 0;
}