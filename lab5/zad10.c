#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>


void down_operation(int semafor_id)
{
	struct sembuf down_operation = {0, -1, 0};
	
	if(semop(semafor_id, &down_operation, 1) < 0)
	{
		perror("Down nie zadziałało");
	}
}
void robotnik1(int semafor_id,int potok[]){
	int i;
		
		for(i = 1; i <= 10; i++)
		{
			if(semctl(semafor_id, 0, GETVAL) == 0)
			{
				break;
			}
			
			write(potok[1], &i, sizeof(int));
		}
		
		close(potok[1]);}
void robotnik2(int semafor_id,int potok[]){
	while(1)
		{
			int temp = 0;
			
			read(potok[0], &temp, sizeof(int));
			printf("Odczytano liczbę: %d\n", temp);
			
			if(temp == 10)
			{
				down_operation(semafor_id);
				
				break;
			}
		}
		
		close(potok[0]);
		
		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
}
int main()
{
	int potok[2];
	pipe(potok);

	int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semafora");
	}
	
	if(semctl(semafor_id, 0, SETVAL, 1) < 0)
	{
		perror("Nie udało się ustawić wartości startowej dla semafora");
	}
	
	
	pid_t potomek = fork();
	
	if(potomek == 0)
	{
		robotnik1(semafor_id,potok);
	
	}
	else
	{		robotnik2(semafor_id,potok);
	
	}

	return 0;
}
