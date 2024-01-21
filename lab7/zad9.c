#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>

#define ELEMENTS_NUMBER 20

int array[ELEMENTS_NUMBER];

struct ThreadData {
    int type;
    int *array;
};

void *thread_function(void *data) {
    struct ThreadData *threadData = (struct ThreadData *)data;
    int type = threadData->type;
    int *array = threadData->array;

    int i, old;
    int return_code = pthread_setcanceltype(type == 1 ? PTHREAD_CANCEL_ASYNCHRONOUS : PTHREAD_CANCEL_DEFERRED, &old);

    if (return_code != 0)
        fprintf(stderr, "pthread_setcanceltype() error: %d\n", return_code);

    for (;;) {
        memset((void *)array, 0, ELEMENTS_NUMBER * sizeof(int));
        for (i = 0; i < ELEMENTS_NUMBER; i++) {
            if (i == ELEMENTS_NUMBER / 2)
                pthread_testcancel();
            array[i] = i;
        }
    }

    return NULL;
}

int main(int argc, char **argv) {
    int type, i, j;
    char output[20];
    if (argc == 2) {
        type = atoi(argv[1]);
        if (type != 1 && type != 2) {
            puts("Niewłaściwa wartość argumentów. Uruchom program bez\
                argumentów, żeby dowiedzieć się więcej.");
            return -1;
        }

        pthread_t thread_id;
        struct ThreadData threadData;
        threadData.type = type;
        threadData.array = array;

        for (j = 0; j < 10; j++) {
            sprintf(output, "Iteracja nr: %d\n", j + 1);
            write(1, output, strlen(output));

            int return_code = pthread_create(&thread_id, NULL, thread_function, (void *)&threadData);
            if (return_code != 0)
                fprintf(stderr, "pthread_create() error: %d\n", return_code);

            if (sched_yield() < 0)
                perror("sched_yield");

            return_code = pthread_cancel(thread_id);
            if (return_code != 0)
                fprintf(stderr, "pthread_cancel() error: %d\n", return_code);

            return_code = pthread_join(thread_id, NULL);
            if (return_code != 0)
                fprintf(stderr, "pthread_join() error: %d\n", return_code);

            for (i = 0; i < ELEMENTS_NUMBER; i++) {
                sprintf(output, "%d ", array[i]);
                write(1, output, strlen(output));
            }
            write(1, "\n", strlen("\n"));
        }
    } else
        puts("Wymaga argumentu wywołania: \n\
        1 - anulowanie asynchroniczne, \n 2 - anulowanie synchroniczne.");

    return 0;
}

