#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
void funkcja(int id,int semafor_id,char *wspoldzielone){
struct shmid_ds bufor;
struct sembuf down_operation = {0, -1, 0};
	
	int i;
	for(i = 0; i < 3; i++)
	{
		if(i == 2)
		{
			if(shmctl(id, IPC_STAT, &bufor)<0)
			perror("shmctl");
			char *mode = "600";
		  	sscanf(mode, "%o", &bufor.shm_perm.mode);
		  	if(shmctl(id, IPC_SET, &bufor)<0)
			perror("shmctl");
		    	printf("Zmieniono prawa dostępu na: %o\n", bufor.shm_perm.mode);
		}
		
		if(semop(semafor_id, &down_operation, 1)<0)
		perror("semop");
	
		printf("Tekst odczytany z obszaru pamięci współdzielonej: %s\n", wspoldzielone);
	}
	
	if(shmctl(id, IPC_RMID, 0)<0)
	perror("shmctl");

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

	char *wspoldzielone = shmat(id, NULL, 0);
	if(*wspoldzielone<0)
	perror("shmat");
	funkcja(id,semafor_id,wspoldzielone);
	/*struct sembuf down_operation = {0, -1, 0};
	
	int i;
	for(i = 0; i < 3; i++)
	{
		if(i == 2)
		{
			shmctl(id, IPC_STAT, &bufor);
			char *mode = "600";
		  	sscanf(mode, "%o", &bufor.shm_perm.mode);
		  	shmctl(id, IPC_SET, &bufor);
		    	printf("Zmieniono prawa dostępu na: %o\n", bufor.shm_perm.mode);
		}
		
		semop(semafor_id, &down_operation, 1);
	
		printf("Tekst odczytany z obszaru pamięci współdzielonej: %s\n", wspoldzielone);
	}
	
	shmctl(id, IPC_RMID, 0);*/
}
