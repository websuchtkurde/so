#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/sendfile.h>

void copy_file(char *source, char *destination) {
    int d = open(source, O_RDONLY);
    int d1 = creat(destination, S_IRWXU);
    off_t roz = lseek(d, 0, SEEK_END);
    lseek(d, 0, SEEK_SET);
    sendfile(d1, d, NULL, roz);
    close(d);
    close(d1);
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Jako pierwszy argument należy podać nazwe pliku do skopiowania, a jako drugi nazwę kopii\n");
        return 0;
    }
    copy_file(argv[1], argv[2]);
    return 0;
}

