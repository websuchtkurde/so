#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

void read_and_write_file(char *filename) {
    int d = open(filename, O_RDONLY);
    off_t roz = lseek(d, 0, SEEK_END);
    char *tmp = mmap(0, roz, PROT_READ, MAP_SHARED, d, 0);
    lseek(d, 0, SEEK_SET);
    read(d, tmp, roz);
    write(1, tmp, roz);
    write(1, "\n", 1);
    close(d);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        write(1, "Jako drugi argument należy podać nazwe pliku\n", 45);
        return 0;
    }
    read_and_write_file(argv[1]);
    return 0;
}
