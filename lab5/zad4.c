#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>

void wait_and_up(int semafor_id, int i)
{
	struct sembuf wait_operation = {0, -i, 0};
	struct sembuf up_operation = {0, i+1, 0};
				
	if(semop(semafor_id, &wait_operation, 1) < 0)
	{
		perror("Wait nie zadziałało");
	}
				
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("Up nie zadziałało");
	}
}

int main()
{
	int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semaforów");
	}

	int potomek[5];
	int i;

	pid_t PID_rodzica = getpid();

	for(i = 0; i < 5; i++)
	{
		//jeśli jest proces rodzica to utworzy potomka
		if(PID_rodzica == getpid())
		{
			//printf("Utworzono\n");
			potomek[i] = fork();
		}
	}

	for(i = 0; i < 5; i++)
	{
		if(potomek[i] == 0)
		{
			wait_and_up(semafor_id, i);
					
			printf("Ustawiono wartosc semafora na: %d\n", semctl(semafor_id, 0, GETVAL));
					
			break;
		}
	}


	if(PID_rodzica == getpid())
	{
		while(wait(NULL) > 0);
		
		struct sembuf wait_operation = {0, -5, 0};

		if(semop(semafor_id, &wait_operation, 1) < 0)
		{
			perror("Nie udalo się wykonać operacji wait dla semafora");
		}

		printf("Wszystkie semafory zakonczyly dzialanie\n");

		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
	}

	return 0;
}
