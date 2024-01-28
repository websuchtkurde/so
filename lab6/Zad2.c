#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <wait.h>
#include <errno.h>

#define SHARED_MEMORY_SIZE 1024



// Funkcja do obsługi semaforów
void sem_operation(int sem_id, int sem_num, int sem_op) {
    struct sembuf sembuf;
    sembuf.sem_num = sem_num;
    sembuf.sem_op = sem_op;
    sembuf.sem_flg = 0;

    if (semop(sem_id, &sembuf, 1) == -1) {
        perror("Błąd operacji na semaforze");
    }
}

int main(void) {
    // Tworzenie obszaru pamięci dzielonej
    int shm_id = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("Błąd tworzenia obszaru pamięci dzielonej");
    }

    // Przyłączanie obszaru pamięci dzielonej do przestrzeni adresowej procesu
    char* shared_memory = shmat(shm_id, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("Błąd przyłączania obszaru pamięci dzielonej");
    }

    // Tworzenie semaforów
    int sem_id = semget(IPC_PRIVATE, 3, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("Błąd tworzenia semaforów");
    }

    // Inicjalizacja semaforów
    semctl(sem_id, 0, SETVAL, 1); // Pierwszy semafor jest dostępny
    semctl(sem_id, 1, SETVAL, 0); // Drugi semafor jest zablokowany
    semctl(sem_id, 2, SETVAL, 0); // Trzeci semafor jest zablokowany

    // Tworzenie procesów
    int i;
    for (i = 0; i < 3; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Błąd fork()");
        } else if (child_pid == 0) {
            // Kod procesu potomnego

            // Zablokowanie semafora
            sem_operation(sem_id, i, -1);

            // Dostęp do obszaru pamięci dzielonej
            printf("Proces %d uzyskał dostęp do obszaru pamięci dzielonej.\n", i);
            sleep(2); // Symulacja pracy w obszarze pamięci dzielonej

            // Odblokowanie semafora
            sem_operation(sem_id, (i + 1) % 3, 1);

            // Oczekiwanie na zakończenie procesu
            exit(0);
        }
    }

    // Oczekiwanie na zakończenie procesów potomnych
    for (i = 0; i < 3; i++) {
        wait(NULL);
    }

    // Usuwanie semaforów
    semctl(sem_id, 0, IPC_RMID, 0);

    // Usuwanie obszaru pamięci dzielonej
    shmdt(shared_memory);
    shmctl(shm_id, IPC_RMID, 0);

    return 0;
}

