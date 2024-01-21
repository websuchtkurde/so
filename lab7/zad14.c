#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int count = 0; // Liczba elementów w buforze
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t producer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t consumer_cond = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            // Bufer jest pełen, czekaj na sygnał od konsumenta
            pthread_cond_wait(&producer_cond, &mutex);
        }

        buffer[count++] = i;
        printf("Producent dodaje: %d (Liczba elementów: %d)\n", i, count);

        // Powiadom konsumenta, że dodano nowy element
        pthread_cond_signal(&consumer_cond);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    for (int i = 0; i < 10; ++i) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            // Bufer jest pusty, czekaj na sygnał od producenta
            pthread_cond_wait(&consumer_cond, &mutex);
        }

        int value = buffer[--count];
        printf("Konsument pobiera: %d (Liczba elementów: %d)\n", value, count);

        // Powiadom producenta, że pobrano element
        pthread_cond_signal(&producer_cond);

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Tworzenie wątków producenta i konsumenta
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Oczekiwanie na zakończenie wątków
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Zwalnianie zasobów
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&producer_cond);
    pthread_cond_destroy(&consumer_cond);

    return 0;
}

