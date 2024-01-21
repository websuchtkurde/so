#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <time.h>

void readFromFile(char *filename)
{	
	int d = open(filename, 0666);
	
	while(1)
	{
		off_t roz = lseek(d, 0, SEEK_END);
		char *tmp = (char *)malloc(roz);
		lseek(d, 0, SEEK_SET);
		
		read(d, tmp, roz);
		printf("Odczytane dane: %s\n", tmp);
			
		free(tmp);
		sleep(1);
	}
	
	close(d);
}

int main()
{
	readFromFile("plik1.txt");
	return 0;
}


