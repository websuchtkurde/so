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

void proces1(int d) {
    flock(d, LOCK_EX);
    lseek(d, 0, SEEK_SET);
    char *tekst1 = "DUPA123 ";
    write(d, tekst1, strlen(tekst1));
    printf("(proces 1) Zapisane dane: %s\n", tekst1);
    flock(d, LOCK_UN);
}

void proces2(int d) {
    flock(d, LOCK_EX);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 2) Odczytane dane: %s\n", tmp);
    free(tmp);
    flock(d, LOCK_UN);
}

void proces3(int d) {
    flock(d, LOCK_EX);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 3) Odczytane dane: %s\n", tmp);
    free(tmp);
    flock(d, LOCK_UN);
}

void proces4(int d) {
    flock(d, LOCK_EX);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 4) Odczytane dane: %s\n", tmp);
    free(tmp);
    flock(d, LOCK_UN);
}

int main() {	
    int d = open("plik1.txt", O_RDWR|O_CREAT, 0666);
    int pid_rodzica = getpid();
    int potomek[4];
    potomek[0] = fork();
    if(pid_rodzica == getpid()) {
        potomek[1] = fork();
        potomek[2] = fork();
        potomek[3] = fork();
    }
    if(potomek[0] == 0) {
        proces1(d);
    } else if(potomek[1] == 0 && potomek[2] != 0 && potomek[3] != 0) {
        proces2(d);
    } else if(potomek[2] == 0 && potomek[1] != 0 && potomek[3] != 0) {
        proces3(d);
    } else if(potomek[3] == 0 && potomek[2] != 0 && potomek[1] != 0) {
        proces4(d);
    }
    if(pid_rodzica == getpid()) {
        while(wait(NULL) > 0);
        close(d);
    }
    return 0;
}
