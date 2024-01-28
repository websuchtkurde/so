#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>


int main()
{
	int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semafora");
	}

	pid_t potomek = fork();

	
	if(potomek != 0)
	{
		struct sembuf wait_operation = {0, 0, 0};
		struct sembuf up_operation = {0, 1, 0};
			
		if(semop(semafor_id, &wait_operation, 1) < 0)
		{
			perror("Wait nie zadziałało");
		}
				
		if(semop(semafor_id, &up_operation, 1) < 0)
		{
			perror("Up nie zadziałało");
		}
				
		printf("Proces 1: Ustawiono wartosc semafora na: %d i czekamy na 2\n", semctl(semafor_id, 0, GETVAL));
		
		struct sembuf wait_operation2 = {0, -2, 0};
			
		if(semop(semafor_id, &wait_operation2, 1) < 0)
		{
			perror("Wait nie zadziałało");
		}
		
		if(semop(semafor_id, &up_operation, 1) < 0)
		{
			perror("Up nie zadziałało");
		}
	}
	else
	{
		sleep(1);
		
		printf("Proces 2: Czekamy na 0 na semaforze.\n");
		struct sembuf wait_operation = {0, 0, 0};

		if(semop(semafor_id, &wait_operation, 1) < 0)
		{
			perror("Nie udalo się wykonać operacji wait dla semafora");
		}

		printf("Otrzymano wartosc 0 na semaforze, więc idziemy dalej.\n");

		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
	}


	return 0;
}

//O to chodzi w ogóle?!
//zrobić podział na funkcje z parametrami
