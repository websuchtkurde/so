#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <wait.h>
#include <unistd.h>
void funkcja(int semafor_id, char *wspoldzielone)
{
	struct sembuf wait_operation = {0, 0, 0};
	struct sembuf up_operation = {0, 1, 0};
	int i;
	for(i = 0; i < 3; i++)
	{
		if(semop(semafor_id, &wait_operation, 1)<0)
		perror("semop");
		
		char toSave[20] = "jakis tekst nr: ";
		char xd = i + '0';
		strcat(toSave, &xd);
		strcpy(wspoldzielone, toSave);
		
		if(semop(semafor_id, &up_operation, 1)<0)
		perror("semop");
		
		sleep(1);
	}

	printf("Koniec\n");
}

int main(void)
{
	int id_pamieci = ftok("zad5A.c", 9);
	if(id_pamieci<0)
	perror("ftok");
	int id_semafora = ftok("zad5A.c", 2);
	if(id_semafora<0)
	perror("ftok");
	int id = shmget(id_pamieci, 50, IPC_CREAT | 0660);
	if(id<0)
	perror("shmget");
	int semafor_id = semget(id_semafora, 1, 0600 | IPC_CREAT);
	if(semafor_id<0)
	perror("semget");
	
	if(semctl(semafor_id, 0, SETVAL, 1)<0)
	perror("semctl");

	char *wspoldzielone = shmat(id, NULL, 0);
	if(*wspoldzielone<0)
	perror("shmat");
	funkcja(semafor_id,wspoldzielone);
	
}

//NAJPIERW URUCHOMIĆ PROGRAM 4B - inaczej się zbuguje i przy kolejnych uruchomieniach nie zadziała poprawnie
