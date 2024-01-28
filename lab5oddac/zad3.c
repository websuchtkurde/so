#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

void wait_for_zero(int semafor_id, int semafor_num)
{
	struct sembuf wait_operation = {semafor_num, 0, 0};

	if(semop(semafor_id, &wait_operation, 1) < 0)
	{
		perror("semop");
	}
}

int main(void)
{
	int semafor_id = semget(IPC_PRIVATE, 10, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("semget");
	}

	int potomek[10];	
	int i;	
	
	
	unsigned short arg[10] = {1,1,1,1,1,1,1,1,1,1};

	
	if(semctl(semafor_id, 0, SETALL, arg) < 0)
	{
		perror("semctl");
	}


	pid_t PID_rodzica = getpid();

	for(i = 0; i < 10; i++)
	{
		
		if(PID_rodzica == getpid())
		{
			
			potomek[i] = fork();
			if(potomek[i]<0)
			perror("fork");
		}
	}

	for(i = 0; i < 10; i++)
	{
		if(potomek[i] == 0)
		{
			
			sleep(1);

			struct sembuf down_operation = {i, -1, 0};

			if(semop(semafor_id, &down_operation, 1) < 0)
			{
				perror("semop");
			}
					
			printf("Ustawiono wartosc semafora %d na: %d\n", i, semctl(semafor_id, i, GETVAL));
					
			break;
		}
	}


	if(PID_rodzica == getpid())
	{
		while(wait(NULL) > 0);
		
		for(i = 0; i < 10; i++)
		{
			wait_for_zero(semafor_id, i);
		}

		printf("Wszystkie semafory zakonczyly dzialanie\n");

		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("semctl");
		}
	}

	return 0;
}
