#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <wait.h>
#define SHARED_MEMORY_SIZE 1024

int sem_id;
char* shared_memory;

// Funkcja do obsługi semaforów
void sem_operation(int sem_num, int sem_op) {
    struct sembuf sembuf;
    sembuf.sem_num = sem_num;
    sembuf.sem_op = sem_op;
    sembuf.sem_flg = 0;

    if (semop(sem_id, &sembuf, 1)<0) {
        perror("semop");
    }
}

// Funkcja czytelnika
void czytelnik(int id) {
    sem_operation(0, -1); // Blokowanie pisarzy
    sem_operation(1, 1);  // Zgłaszanie wejścia do sekcji krytycznej dla czytelników

    // Odczytanie wiadomości
    char message[SHARED_MEMORY_SIZE];
    strncpy(message, shared_memory, SHARED_MEMORY_SIZE);

    sem_operation(1, -1); // Zwolnienie sekcji krytycznej dla czytelników
    sem_operation(0, 1);  // Zwolnienie pisarzy

    // Wypisanie odczytanej wiadomości
    printf("Czytelnik %d odczytał: %s\n", id, message);
}

// Funkcja pisarza
void pisarz(int id) {
    sem_operation(0, -1); // Blokowanie czytelników
    sem_operation(2, 1);  // Zgłaszanie wejścia do sekcji krytycznej dla pisarzy

    // Zapis wiadomości
    char message[SHARED_MEMORY_SIZE];
    sprintf(message, "Wiadomość od pisarza %d", id);
    strncpy(shared_memory, message, SHARED_MEMORY_SIZE);

    sem_operation(2, -1); // Zwolnienie sekcji krytycznej dla pisarzy
    sem_operation(0, 1);  // Zwolnienie czytelników
}

int main() {
    int shm_id;
    // Tworzenie lub uzyskanie dostępu do obszaru pamięci dzielonej
    key_t key = ftok("/tmp", 's');
    shm_id = shmget(key, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget");
    }

    // Przyłączanie obszaru pamięci dzielonej do przestrzeni adresowej procesu
    shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat");
    }

    // Tworzenie semaforów
    sem_id = semget(key, 3, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget");
    }

    // Inicjalizacja semaforów
    if(semctl(sem_id, 0, SETVAL, 1)<0)
    perror("semctl"); // Semafor dla czytelników/pisarzy
    if(semctl(sem_id, 1, SETVAL, 1)<0)
    perror("semctl"); // Semafor dla sekcji krytycznej dla czytelników
    if(semctl(sem_id, 2, SETVAL, 1)<0)
    perror("semctl"); // Semafor dla sekcji krytycznej dla pisarzy

    // robienie czytelnika i pisarza
    int i;
    for (i = 0; i < 3; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
        } else if (child_pid == 0) {
            if (i % 2 == 0) {
                // Czytelnik
                czytelnik(i);
            } else {
                // Pisarz
                pisarz(i);
            }
            exit(0);
        }
    }

    // czekanie az sie potomne skoncza
    for (i = 0; i < 3; i++) {
        wait(NULL);
    }

    // rem sem
    if(semctl(sem_id, 0, IPC_RMID, 0)<0)
    perror("semctl");

    // rem shm
    if(shmdt(shared_memory)<0)
    perror("shmdt");
    if(shmctl(shm_id, IPC_RMID, 0)<0)
    perror("shmctl");

    return 0;
}

