#include <stdio.h>

int main() {
    FILE *fp;
    char *filename = "send.txt";

    // Otwieranie pliku
    fp = fopen(filename, "w");
    if(fp == NULL) {
        perror(" - Error in opening file. Exiting . . .");
        exit(1);
    }

    // Wypełnianie pliku danymi do momentu, gdy jego rozmiar przekroczy 1 MiB
    for(int i = 0; i < (1536 * 1536); i++) {
        fputc('A', fp);
    }

    fclose(fp);

    printf(" + File created and written successfully.\n");

    return 0;
}
