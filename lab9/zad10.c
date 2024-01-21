#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "numbers.txt"

int main() {
    int fd = open(FILENAME, O_CREAT | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    srand(time(NULL));

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    for (int i = 0; i < 100; i++) {
        int num = rand() % 100;

        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror("fcntl");
            return 1;
        }

        dprintf(fd, "%d\n", num);
        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        sleep(1);
    }

    close(fd);

    return 0;
}
