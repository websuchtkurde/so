#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>

#define READERS_COUNT 5
#define WRITERS_COUNT 2

pthread_t readers[READERS_COUNT];
pthread_t writers[WRITERS_COUNT];
int reader_ids[READERS_COUNT];
int writer_ids[WRITERS_COUNT];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t resource_semaphore;
int resource_data = 0;

int running = 1;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *reader(void *arg) {
    int reader_id = *((int *)arg);

    while (1) {
        pthread_mutex_lock(&mutex);

        // Sprawdź warunek zakończenia
        if (!running) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);

        // Czytaj dane
        printf("Czytelnik %d czyta: %d\n", reader_id, resource_data);

        // Poczekaj przed ponownym czytaniem
        usleep(rand() % 1000000);
    }

    pthread_exit(NULL);
}

void *writer(void *arg) {
    int writer_id = *((int *)arg);

    while (1) {
        sem_wait(&resource_semaphore);

        // Sprawdź warunek zakończenia
        if (!running) {
            sem_post(&resource_semaphore);
            break;
        }

        // Zapisz dane
        resource_data++;
        printf("Pisarz %d pisze: %d\n", writer_id, resource_data);

        // Zakończ zapis
        sem_post(&resource_semaphore);

        // Poczekaj przed ponownym zapisem
        usleep(rand() % 1000000);
    }

    pthread_exit(NULL);
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        printf("\nReceived SIGINT. Cleaning up and exiting.\n");

        // Ustaw flagę końca działania programu
        pthread_mutex_lock(&mutex);
        running = 0;
        pthread_cond_broadcast(&cond);  // Powiadom wszystkie wątki o zakończeniu
        pthread_mutex_unlock(&mutex);

        // Zakończ wątki
        for (int i = 0; i < READERS_COUNT; ++i) {
            pthread_join(readers[i], NULL);
        }

        for (int i = 0; i < WRITERS_COUNT; ++i) {
            pthread_join(writers[i], NULL);
        }

        // Zwalnianie zasobów
        pthread_mutex_destroy(&mutex);
        sem_destroy(&resource_semaphore);
        pthread_cond_destroy(&cond);

        exit(EXIT_SUCCESS);
    }
}

int main() {
    // Inicjalizacja semafora
    sem_init(&resource_semaphore, 0, 1);

    // Inicjalizacja obsługi sygnałów
    signal(SIGINT, signal_handler);

    // Tworzenie wątków czytelników
    for (int i = 0; i < READERS_COUNT; ++i) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, (void *)&reader_ids[i]);
    }

    // Tworzenie wątków pisarzy
    for (int i = 0; i < WRITERS_COUNT; ++i) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, (void *)&writer_ids[i]);
    }

    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < READERS_COUNT; ++i) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < WRITERS_COUNT; ++i) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}

