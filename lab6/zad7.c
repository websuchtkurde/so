#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
void robotnik1(int semafor_id, char *wspoldzieloneA){
	struct sembuf wait_operation1 = {0, 0, 0};
	struct sembuf up_operation1 = {0, 1, 0};
	semop(semafor_id, &wait_operation1, 1);
		
		char *toSave = "tekst z procesu 1";
		strcpy(wspoldzieloneA, toSave);
		
		printf("(proces 1) Zapisano do pamięci współdzielonej A tekst: %s\n", toSave);
		
		sleep(2);
		
		printf("(proces 1) Odczytano z pamięci współdzielonej A tekst: %s\n", wspoldzieloneA);
		
		semop(semafor_id, &up_operation1, 1);

}
void robotnik2(int semafor_id, char *wspoldzieloneA, char *wspoldzieloneB){
	struct sembuf wait_operation2 = {1, 0, 0};
	struct sembuf up_operation2 = {1, 1, 0};
	semop(semafor_id, &wait_operation2, 1);
		
		char *temp = malloc(20);
		strcpy(temp, wspoldzieloneB);
		
		strcpy(wspoldzieloneB, wspoldzieloneA);
		strcpy(wspoldzieloneA, temp);
		
		semop(semafor_id, &up_operation2, 1);
}
void robotnik3(int semafor_id, char *wspoldzieloneB){
	struct sembuf wait_operation3 = {2, 0, 0};
	struct sembuf up_operation3 = {2, 1, 0};

	semop(semafor_id, &wait_operation3, 1);
		
		char *toSave = "tekst z procesu 3";
		strcpy(wspoldzieloneB, toSave);
		
		printf("(proces 3) Zapisano do pamięci współdzielonej B tekst: %s\n", toSave);
		
		sleep(2);
		
		printf("(proces 3) Odczytano z pamięci współdzielonej B tekst: %s\n", wspoldzieloneB);
		
		semop(semafor_id, &up_operation3, 1);
}
int main(void)
{
	int semafor_id = semget(IPC_PRIVATE, 3, 0600 | IPC_CREAT);
	
	//struct sembuf wait_operation1 = {0, 0, 0};
	//struct sembuf up_operation1 = {0, 1, 0};
	struct sembuf down_operation1 = {0, -1, 0};
	
	//struct sembuf wait_operation2 = {1, 0, 0};
	//struct sembuf up_operation2 = {1, 1, 0};
	struct sembuf down_operation2 = {1, -1, 0};
	
	//struct sembuf wait_operation3 = {2, 0, 0};
	//struct sembuf up_operation3 = {2, 1, 0};
	struct sembuf down_operation3 = {2, -1, 0};
	
	semctl(semafor_id, 0, SETVAL, 1);
	semctl(semafor_id, 1, SETVAL, 1);
	semctl(semafor_id, 2, SETVAL, 1);
	
	int idA = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	int idB = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	
	char *wspoldzieloneA = shmat(idA, NULL, 0);
	char *wspoldzieloneB = shmat(idB, NULL, 0);
	
	int pid_rodzica = getpid();
	
	int potomek[3];
	potomek[0] = fork();
	
	if(pid_rodzica == getpid())
	{
		potomek[1] = fork();
		potomek[2] = fork();
	}
	
	//proces 1
	if(potomek[0] == 0)
	{
		robotnik1(semafor_id,wspoldzieloneA);
		
	}
	
	//proces 2
	else if(potomek[1] == 0 && potomek[2] != 0)
	{
		robotnik2(semafor_id, wspoldzieloneA, wspoldzieloneB);
		
	}
	
	//proces 3
	else if(potomek[2] == 0 && potomek[1] != 0)
	{		
		robotnik3(semafor_id, wspoldzieloneB);
		
	}
	
	//proces 4
	if(pid_rodzica == getpid())
	{				
		semop(semafor_id, &down_operation1, 1);
		semop(semafor_id, &down_operation3, 1);
		sleep(1);
		semop(semafor_id, &down_operation2, 1);
		while(wait(NULL) > 0);
		semctl(semafor_id, 0, IPC_RMID);
		shmctl(idA, IPC_RMID, 0);
		shmctl(idB, IPC_RMID, 0);
	}
	
	return 0;
}
