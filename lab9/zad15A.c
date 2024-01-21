#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <time.h>

void writeToFile(char *filename, char *tekst1)
{	
	int d = open(filename, 0666);
	
	while(1)
	{
		lseek(d, 0, SEEK_SET);	
		write(d, tekst1, strlen(tekst1));
		printf("Zapisano do pliku tekst: %s\n", tekst1);
		sleep(1);
	}
	
	close(d);
}

int main()
{
	writeToFile("plik1.txt", "HALO");
	return 0;
}


//można zrobić żeby w każdej iteracji wpisywało coś innego, np dopisywało cyfre
