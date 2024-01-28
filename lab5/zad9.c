#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

struct msgbuf
{
	long mtype;
	char mtext[50];
} w1, w2;


void up_and_down_parent(int semafor_id)
{
	struct sembuf up_operation = {1, 1, 0};
	struct sembuf down_operation = {0, -1, 0};
	
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("Up nie zadziałało");
	}
		
	if(semop(semafor_id, &down_operation, 1) < 0)
	{
		perror("Down nie zadziałało");
	}
}

void up_and_down_child(int semafor_id)
{
	struct sembuf up_operation = {0, 1, 0};
	struct sembuf down_operation = {1, -1, 0};
	
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("Up nie zadziałało");
	}
		
	if(semop(semafor_id, &down_operation, 1) < 0)
	{
		perror("Down nie zadziałało");
	}
}


int main()
{
	int kolejka_id = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
	if(kolejka_id < 0)
	{
		perror("Nie udalo sie utworzyć kolejki komunikatów");
	}
	
	int i;
	
	int semafor_id = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT);

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semaforów");
	}
	
	if(semctl(semafor_id, 1, SETVAL, 1) < 0)
	{
		perror("Nie udało się ustawić wartości startowej dla semafora");
	}
	
	
	pid_t potomek = fork();
	
	if(potomek == 0)
	{		
		struct sembuf wait_operation = {0, 0, 0};
		
		sleep(1);
		
		for(i = 0; i < 20; i++)
		{
			if(semop(semafor_id, &wait_operation, 1) < 0)
			{
				perror("Wait nie zadziałało");
			}
			
			w1.mtype = 1;
			strcpy(w1.mtext, "jakiś tekst");
			
			if(msgsnd(kolejka_id, &w1, 50, IPC_NOWAIT) < 0)
			{
				perror("Nie udało się wysłać komunikatu");
			}
			
			up_and_down_child(semafor_id);
		}
	}
	else
	{
		struct sembuf wait_operation = {1, 0, 0};
		
		for(i = 0; i < 20; i++)
		{
			if(semop(semafor_id, &wait_operation, 1) < 0)
			{
				perror("Wait nie zadziałało");
			}	
			
			if(msgrcv(kolejka_id, &w2, 50, 0, IPC_NOWAIT) < 0)
			{
				perror("Błąd odbierania wiadomości z kolejki");
			}
			else
			{
				printf("Odebrana wiadomość: %s\n", w2.mtext);
			}
			
			up_and_down_parent(semafor_id);
		}
		
		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
		
		if(msgctl(kolejka_id, IPC_RMID, 0) < 0)
		{
			perror("Nie udało się usunąć kolejki komunikatów");
		}
	}

	return 0;
}
