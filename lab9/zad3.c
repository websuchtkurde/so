#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

void odczytajPlik(char* nazwaPliku) {
	int plik1, plik2;
	char buff[0xFFF] = "";
	
	plik1 = open(nazwaPliku, O_RDWR, 0777);
	
	plik2 = dup(plik1);
	close(plik1);
	
	lseek(plik2, 0, SEEK_SET);
	read(plik2, buff, 0xFFF);
	
	printf("Tekst odczytany z pliku dowiązanego do skopiowanego deskryptora: %s\n", buff);
	
	close(plik2);
}

int main() {
	odczytajPlik("plik.txt");
	return 0;
}
