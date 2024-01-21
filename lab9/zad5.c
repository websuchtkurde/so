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

void proces1(int d, struct flock lock, struct flock unlock) {
    fcntl(d, F_SETLK, &lock);
    lseek(d, 0, SEEK_SET);
    char *tekst1 = "Jakiś tam tekst1 ";
    write(d, tekst1, strlen(tekst1));
    printf("(proces 1) Zapisane dane: %s\n", tekst1);
    fcntl(d, F_SETLK, &unlock);
}

void proces2(int d, struct flock lock, struct flock unlock) {
    fcntl(d, F_SETLK, &lock);
    lseek(d, 0, SEEK_SET);
    char *tekst2 = "Jakiś tam tekst2 ";
    write(d, tekst2, strlen(tekst2));
    printf("(proces 2) Zapisane dane: %s\n", tekst2);
    fcntl(d, F_SETLK, &unlock);
}

void proces3(int d, struct flock lock, struct flock unlock) {
    fcntl(d, F_SETLK, &lock);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 3) Odczytane dane: %s\n", tmp);
    free(tmp);
    fcntl(d, F_SETLK, &unlock);
}

void proces4(int d, struct flock lock, struct flock unlock) {
    fcntl(d, F_SETLK, &lock);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 4) Odczytane dane: %s\n", tmp);
    free(tmp);
    fcntl(d, F_SETLK, &unlock);
}

void proces5(int d, struct flock lock, struct flock unlock) {
    fcntl(d, F_SETLK, &lock);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = (char *)malloc(roz);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    printf("(proces 5) Odczytane dane: %s\n", tmp);
    free(tmp);
    fcntl(d, F_SETLK, &unlock);
}

int main() {	
    int d = open("plik1.txt", O_RDWR|O_CREAT, 0666);
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 5;
    struct flock unlock;
    unlock.l_type = F_UNLCK;
    unlock.l_whence = SEEK_SET;
    unlock.l_start = 0;
    unlock.l_len = 5;
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
        proces1(d, lock, unlock);
    } else if(potomek[1] == 0 && potomek[2] != 0 && potomek[3] != 0 && potomek[4] != 0) {
        proces2(d, lock, unlock);
    } else if(potomek[2] == 0 && potomek[1] != 0 && potomek[3] != 0 && potomek[4] != 0) {
        proces3(d, lock, unlock);
    } else if(potomek[3] == 0 && potomek[2] != 0 && potomek[1] != 0 && potomek[4] != 0) {
        proces4(d, lock, unlock);
    } else if(potomek[4] == 0 && potomek[2] != 0 && potomek[1] != 0 && potomek[3] != 0) {
        proces5(d, lock, unlock);
    }
    if(pid_rodzica == getpid()) {
        while(wait(NULL) > 0);
        close(d);
    }
    return 0;
}
