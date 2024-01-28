#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>


void wait_and_up(int semafor_id)
{
	struct sembuf wait_operation = {0, 0, 0};
	//SEM_UNDO powoduje że operacja zostanie cofnięta po zakończeniu procesu, który ją wykonał
	struct sembuf up_operation = {0, 1, SEM_UNDO};
			
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
		perror("Nie udalo sie utworzyć semafora");
	}

	pid_t potomek = fork();
	
	if(potomek == 0)
	{
		wait_and_up(semafor_id);
				
		printf("Proces 1: Ustawiono wartosc semafora na: %d\n", semctl(semafor_id, 0, GETVAL));
		
		sleep(2);
	}
	else
	{
		sleep(1);

		printf("Proces potomny jeszcze żyje, a wartość semafora wynosi: %d\n", semctl(semafor_id, 0, GETVAL));
		
		wait(NULL);
		
		printf("Proces potomny zakonczyl dzialanie, a wartość semafora wynosi: %d\n", semctl(semafor_id, 0, GETVAL));

		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
	}


	return 0;
}

//być może to jest źle