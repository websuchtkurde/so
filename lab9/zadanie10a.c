#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

#define FILENAME "numbers.txt"

int main() {
    int fd = open(FILENAME, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    int max = INT_MIN;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;

    while (1) {
        int num;

        if (fcntl(fd, F_SETLKW, &lock) == -1) {
            perror("fcntl");
            return 1;
        }

        if (fscanf(fd, "%d", &num) == 1) {
            if (num > max) {
                max = num;
                printf("Nowa maksymalna wartość: %d\n", max);
            }
        } else {
            clearerr(fd);
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        sleep(1);
    }

    close(fd);

    return 0;
}
