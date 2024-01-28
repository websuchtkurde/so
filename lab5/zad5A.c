#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

void wait_and_up(int semafor_id)
{
	struct sembuf wait_operation = {0, 0, 0};
	struct sembuf up_operation = {0, 1, 0};
	
	//czeka aż wartością będzie 0 (ale 0 to też wartość startowa, więc od razu idzie dalej)		
	if(semop(semafor_id, &wait_operation, 1) < 0)
	{
		perror("Wait nie zadziałało");
	}
	
	//ustawia wartość 1 i tym samym daje sygnał drugiego programowi, że może przechodzić dalej			
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("Up nie zadziałało");
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
	
	wait_and_up(semafor_id);
			
	int kolejka = open("kolejka", O_WRONLY);
	
	char *toSave = "Tekst do zahfghfghpisania";
	
	write(kolejka, toSave, 20);
	
	close(kolejka);

	return 0;
}
