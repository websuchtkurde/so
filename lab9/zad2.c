#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void zapiszDoPliku(char* nazwaPliku) {
	int plik_wyjsciowy;
	
	//utworzenie pliku wynikowego - tryby oznaczają: 
	//O_WRONLY - do zapisu
	//O_CREAT - utworzyć
	//O_TRUNC - nadpisać jeśli istnieje
	plik_wyjsciowy = open(nazwaPliku, O_WRONLY|O_CREAT|O_TRUNC);
	
	char *tekst1 = "Początek ";
	write(plik_wyjsciowy, tekst1, strlen(tekst1));
	
	//ustawienie pozycji wskaźnika pliku na miejsce 504 licząc od początku
	lseek(plik_wyjsciowy, 504, SEEK_SET);
	
	char *tekst2 = " Koniec.";
	write(plik_wyjsciowy, tekst2, strlen(tekst2));
	
	close(plik_wyjsciowy);
}

int main() {
	zapiszDoPliku("plikkkk.txt");
	return 0;
}

