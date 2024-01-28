#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

void wait_and_increase(int semafor_id)
{
	struct sembuf wait_operation = {0, 0, 0};
	struct sembuf up_operation = {0, 1, 0};
	
	if(semop(semafor_id, &wait_operation, 1) < 0)
	{
		perror("Wait nie poszlo");
	}
			
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("Up nie poszlo");
	}
}

int main()
{
	struct sembuf down_operation = {0, -1, 0};
		
	int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);
		
	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semafora");
	}
			
	pid_t potomek = fork();
	int i;
	
	if(potomek == 0)
	{		
		for(i = 0; i < 50; i++)
		{
			wait_and_increase(semafor_id);
			
			
			printf("Jestem procesem potomnym.\n");
			
			
			if(semop(semafor_id, &down_operation, 1) < 0)
			{
				perror("P - Down nie poszlo");
			}
		}
	}
	else
	{	
		sleep(1);
		
		for(i = 0; i < 50; i++)
		{
			wait_and_increase(semafor_id);
			
			
			printf("Jestem procesem rodzica.\n");
			
			
			if(semop(semafor_id, &down_operation, 1) < 0)
			{
				perror("R - Down nie poszlo");
			}	
		}
		
		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
	}
	
	return 0;
}
