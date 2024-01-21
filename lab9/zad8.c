#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <wait.h>

void proces1(int d, int semafor_id, struct sembuf wait_operation, struct sembuf up_operation, struct sembuf down_operation) {
    semop(semafor_id, &wait_operation, 1);
    semop(semafor_id, &up_operation, 1);
    lseek(d, 0, SEEK_SET);
    char *tekst1 = "Jakiś tam tekst1 ";
    write(d, tekst1, strlen(tekst1));
    printf("(proces 1) Zapisane dane: %s\n", tekst1);
    semop(semafor_id, &down_operation, 1);
}

void proces2(int d, int semafor_id, struct sembuf wait_operation, struct sembuf up_operation, struct sembuf down_operation) {
    semop(semafor_id, &wait_operation, 1);
    semop(semafor_id, &up_operation, 1);
    lseek(d, 0, SEEK_SET);
    char *tekst2 = "Jakiś tam tekst2 ";
    write(d, tekst2, strlen(tekst2));
    printf("(proces 2) Zapisane dane: %s\n", tekst2);
    semop(semafor_id, &down_operation, 1);
}

void proces3(int d, int semafor_id, struct sembuf wait_operation, struct sembuf up_operation, struct sembuf down_operation) {
    semop(semafor_id, &wait_operation, 1);
    semop(semafor_id, &up_operation, 1);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 3) Odczytane dane: %s\n", tmp);
    free(tmp);
    semop(semafor_id, &down_operation, 1);
}

void proces4(int d, int semafor_id, struct sembuf wait_operation, struct sembuf up_operation, struct sembuf down_operation) {
    semop(semafor_id, &wait_operation, 1);
    semop(semafor_id, &up_operation, 1);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 4) Odczytane dane: %s\n", tmp);
    free(tmp);
    semop(semafor_id, &down_operation, 1);
}

void proces5(int d, int semafor_id, struct sembuf wait_operation, struct sembuf up_operation, struct sembuf down_operation) {
    semop(semafor_id, &wait_operation, 1);
    semop(semafor_id, &up_operation, 1);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 5) Odczytane dane: %s\n", tmp);
    free(tmp);
    semop(semafor_id, &down_operation, 1);
}

int main() {	
    struct sembuf wait_operation = {0, 0, 0};
    struct sembuf up_operation = {0, 1, 0};
    struct sembuf down_operation = {0, -1, 0};
    int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);
    int d = open("plik1.txt", O_RDWR|O_CREAT, 0666);
    int pid_rodzica = getpid();
    int potomek[5];
    potomek[0] = fork();
    if(pid_rodzica == getpid()) {
        potomek[1] = fork();
        potomek[2] = fork();
        potomek[3] = fork();
        potomek[4] = fork();
    }
    if(potomek[0] == 0) {
        proces1(d, semafor_id, wait_operation, up_operation, down_operation);
    } else if(potomek[1] == 0 && potomek[2] != 0 && potomek[3] != 0 && potomek[4] != 0) {
        proces2(d, semafor_id, wait_operation, up_operation, down_operation);
    } else if(potomek[2] == 0 && potomek[1] != 0 && potomek[3] != 0 && potomek[4] != 0) {
        proces3(d, semafor_id, wait_operation, up_operation, down_operation);
    } else if(potomek[3] == 0 && potomek[2] != 0 && potomek[1] != 0 && potomek[4] != 0) {
        proces4(d, semafor_id, wait_operation, up_operation, down_operation);
    } else if(potomek[4] == 0 && potomek[2] != 0 && potomek[1] != 0 && potomek[3] != 0) {
        proces5(d, semafor_id, wait_operation, up_operation, down_operation);
    }
    if(pid_rodzica == getpid()) {
        while(wait(NULL) > 0);
        close(d);
        semctl(semafor_id, 0, IPC_RMID);
    }
    return 0;
}

