#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

void kopiuj(char *skad, char *dokad) {
    int plik1 = open(skad, O_RDONLY);
    if(plik1<0)
    perror("open");

    int plik2 = creat(dokad, 0777);
    if(plik2<0)
    perror("creat");

    char tmp[1024];
    ssize_t liczba_bajtow;

    while((liczba_bajtow = read(plik1, tmp, sizeof(tmp))) > 0) {
        if(write(plik2, tmp, liczba_bajtow)<0)
        perror("write");
    }

    if(liczba_bajtow<0)
    perror("read");

    if(close(plik1)<0){
    perror("close");
    puts("tyczy sie to plik1");
    }

    if(close(plik2)<0)
    {
    perror("close");
    puts("tyczy sie to plik2");}
}

int main(int argc, char **argv) {
    if(argc != 3) {
        puts("Jako pierwszy argument daj nazwe pliku skad mam skopiowac a jako drugi daj nazwe pliku do ktorego mam skopiowac, w razie czego to zrobie drugi plik");
        return 0;
    }
    kopiuj(argv[1], argv[2]);
    return 0;
}

