#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
void robotnik1(int semafor_id, char *wspoldzielone){
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {0, 0, 0};
		struct sembuf flag_readed = {0, 1, 0};
		int i;
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 1) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			//wypisuje na ekran - normalnie robi się printfem, ale w poleceniu kazali użyć write
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}
}
void robotnik2(int semafor_id, char *wspoldzielone)
{
		int i;
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {1, 0, 0};
		struct sembuf flag_readed = {1, 1, 0};
		
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 2) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}
}
void robotnik3(int semafor_id, char *wspoldzielone)
{		int i;
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {2, 0, 0};
		struct sembuf flag_readed = {2, 1, 0};
		
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 3) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}
}
void robotnik4(int semafor_id, char *wspoldzielone, int id){

int i;
	for(i = 0; i < 10; i++)
		{
			struct sembuf up_operation = {3, 1, 0};
			struct sembuf down_operation = {3, -1, 0};
		
			if(i > 0)
			{
				semop(semafor_id, &up_operation, 1);
			}
			
			int j;
			for(j = 0; j < 3; j++)
			{
				struct sembuf wait_for_readers = {j, -1, 0};
				semop(semafor_id, &wait_for_readers, 1);
			}	
			
			char toSave[20] = "(pisarz) tekst nr ";
			char xd = i + '0';
			strcat(toSave, &xd);
			strcpy(wspoldzielone, toSave);
			
			char output[100] = "\n(proces 4 - pisarz) Zapisano do pamięci współdzielonej tekst: ";
			strcat(output, toSave);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &down_operation, 1);
			
			sleep(1);
		}
		
		while(wait(NULL) > 0);
		
		semctl(semafor_id, 0, IPC_RMID);
		shmctl(id, IPC_RMID, 0);

}
int main()
{
	int semafor_id = semget(IPC_PRIVATE, 4, 0600 | IPC_CREAT);
	
	semctl(semafor_id, 0, SETVAL, 1);
	semctl(semafor_id, 1, SETVAL, 1);
	semctl(semafor_id, 2, SETVAL, 1);
	semctl(semafor_id, 3, SETVAL, 1);
	
	int id = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	char *wspoldzielone = shmat(id, NULL, 0);
	
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
		robotnik1(semafor_id,wspoldzielone);
		/*
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {0, 0, 0};
		struct sembuf flag_readed = {0, 1, 0};
	
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 1) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			//wypisuje na ekran - normalnie robi się printfem, ale w poleceniu kazali użyć write
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}
		*/
	}
	
	//proces 2
	else if(potomek[1] == 0 && potomek[2] != 0)
	{
		robotnik2(semafor_id,wspoldzielone);
		/*
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {1, 0, 0};
		struct sembuf flag_readed = {1, 1, 0};
		
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 2) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}*/
	}
	
	//proces 3
	else if(potomek[2] == 0 && potomek[1] != 0)
	{	
		robotnik3(semafor_id,wspoldzielone);
		/*	
		struct sembuf wait_for_writer = {3, 0, 0};
		struct sembuf wait_operation = {2, 0, 0};
		struct sembuf flag_readed = {2, 1, 0};
		
		for(i = 0; i < 10; i++)
		{
			semop(semafor_id, &wait_for_writer, 1);
			semop(semafor_id, &wait_operation, 1);
			
			char output[100] = "(proces 3) Odczytano z pamięci współdzielonej tekst: ";
			strcat(output, wspoldzielone);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &flag_readed, 1);
		}*/
	}
	
	//proces 4 - pisarz
	if(pid_rodzica == getpid())
	{
		robotnik4(semafor_id, wspoldzielone, id);
		/*
		for(i = 0; i < 10; i++)
		{
			struct sembuf up_operation = {3, 1, 0};
			struct sembuf down_operation = {3, -1, 0};
		
			if(i > 0)
			{
				semop(semafor_id, &up_operation, 1);
			}
			
			int j;
			for(j = 0; j < 3; j++)
			{
				struct sembuf wait_for_readers = {j, -1, 0};
				semop(semafor_id, &wait_for_readers, 1);
			}	
			
			char toSave[20] = "(pisarz) tekst nr ";
			char xd = i + '0';
			strcat(toSave, &xd);
			strcpy(wspoldzielone, toSave);
			
			char output[100] = "\n(proces 4 - pisarz) Zapisano do pamięci współdzielonej tekst: ";
			strcat(output, toSave);
			strcat(output, "\n");
			
			write(1, output, strlen(output));
			
			semop(semafor_id, &down_operation, 1);
			
			sleep(1);
		}
		
		while(wait(NULL) > 0);
		
		semctl(semafor_id, 0, IPC_RMID);
		shmctl(id, IPC_RMID, 0);
		*/
	}
	
	return 0;
}
