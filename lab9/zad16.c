#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

void print_file_info(const char *file_name) {
	int plik = open(file_name, O_RDONLY);
	
	struct stat bufor;
	fstat(plik, &bufor);
	
	printf("ID urządzenia: %ld\n", bufor.st_dev);
	printf("Ilość twardych dowiązań: %ld\n", bufor.st_nlink);
	printf("ID właściciela: %d\n", bufor.st_uid);
	printf("ID grupy właściciela: %d\n", bufor.st_gid);
	printf("Rozmiar pliku: %ld\n", bufor.st_size);
	printf("Ilość zaalokowanych sektorów: %ld\n", bufor.st_blocks);
	
	struct tm *info1;
	struct tm *info2;
	info1 = localtime(&bufor.st_atime);
	info2 = localtime(&bufor.st_mtime);
	char *data1 = malloc(50);
	char *data2 = malloc(50);
	strftime(data1, 50, "%x - %I:%M%p", info1);
	strftime(data2, 50, "%x - %I:%M%p", info2);
	
	printf("Data ostatniego dostępu: %s\n", data1);
	printf("Data ostatniej modyfikacji: %s\n", data2);
	
	free(data1);
	free(data2);
	
	close(plik);
}

int main() {
	print_file_info("plik1.txt");
	return 0;
}
