#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void czytaj_i_zapisz(int plik_wejsciowy, int plik_wyjsciowy, int *bufor, int rozmiar, char *tekst, int koniec) {
    if (koniec) {
        lseek(plik_wejsciowy, -(sizeof(char)*rozmiar), SEEK_END);
    }
    read(plik_wejsciowy, bufor, rozmiar*sizeof(char));
    write(plik_wyjsciowy, tekst, strlen(tekst));
    write(plik_wyjsciowy, bufor, rozmiar*sizeof(char));
}

int main(void)
{
    int plik_wejsciowy, plik_wyjsciowy;
    int bufor[41];
    
    plik_wejsciowy = open("plik.txt", 0666);
    plik_wyjsciowy = open("plik2.txt", O_WRONLY|O_CREAT|O_TRUNC);
    
    char *tekst1 = "Pierwsze 40 znaków:\n";
    czytaj_i_zapisz(plik_wejsciowy, plik_wyjsciowy, bufor, 40, tekst1, 0);
    
    lseek(plik_wyjsciowy, -1, SEEK_END);
    
    char *tekst2 = "\n\nOstatnie 40 znaków:\n";
    czytaj_i_zapisz(plik_wejsciowy, plik_wyjsciowy, bufor, 40, tekst2, 1);
    
    close(plik_wejsciowy);
    close(plik_wyjsciowy);
    
    return 0;
}
