#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

void wait_operation(int semafor_id)
{
	struct sembuf wait_operation = {0, -1, 0};
		
	//czeka na wartość 1 na semaforze, po czym przechodzi dalej		
	if(semop(semafor_id, &wait_operation, 1) < 0)
	{
		perror("Wait nie zadziałało");
	}
}

int main()
{
	mkfifo("kolejka", 0600);
	
	int id = ftok("zad5A.c", 15);
	
	int semafor_id = semget(id, 1, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semaforów");
	}
	
	wait_operation(semafor_id);
	
	int kolejka = open("kolejka", O_RDONLY);
	
	char *readed = malloc(20);
	read(kolejka, readed, 20);
	
	printf("Odczytany tekst: %s\n", readed);
	
	close(kolejka);
	unlink("kolejka");

	return 0;
}
