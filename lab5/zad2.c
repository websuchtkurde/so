#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

void wait_and_increase_child(int semafor_id)
{
	//pierwszy parametr: indeks semafora
	//drugi parametr: jeśli 1 to przypisze 1, jeśli zero to czeka na wyzerowanie, a jeśli -1 to odejmie 1
	//trzeci parametr: flaga (np. IPC_NOWAIT)
	struct sembuf wait_for_zero = {1, 0, 0};
	struct sembuf up_operation = {1, 1, 0};
	
	
	//czeka aż semafor 1 będzie miał wartość 0, wówczas przejdzie dalej			
	//trzeci parametr określa ilość elementów w tablicy semafora)
	if(semop(semafor_id, &wait_for_zero, 1) < 0)
	{
		perror("P -Wait nie poszlo");
	}
	
	//doda wartość jeden do semafora 1 - zablokuje dalsze wykonanie pętli potomka
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("P - Up nie poszlo");
	}
}

void wait_and_increase_parent(int semafor_id)
{
	//stworzenie struktur dla operacji na semaforach, nazwy DOWOLNE
	struct sembuf wait_for_zero = {0, 0, 0};
	struct sembuf up_operation = {0, 1, 0};
	
	
	//czeka aż semafor 0 będzie miał wartość 0 i wtedy przejdzie dalej
	//wartością stertową jest 0, więc w pierwszej pętli od razu pójdzie dalej
	if(semop(semafor_id, &wait_for_zero, 1) < 0)
	{
		perror("R - Wait nie poszlo");
	}
	
	//doda wartość 1 do semafora 0 - zablokuje dalsze wykonani pętli rodzica
	if(semop(semafor_id, &up_operation, 1) < 0)
	{
		perror("R - Up nie poszlo");
	}
}

int main()
{
	//drugi parametr semget oznacza ilość semaforów do utworzenia
	int semafor_id = semget(IPC_PRIVATE, 2, 0600 | IPC_CREAT);
		
	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semaforów");
	}
	
	//ustawienie wartości startowej 1 dla semafora 0
	if(semctl(semafor_id, 0, SETVAL, 1) < 0)
	{
		perror("Nie udało się przydzielić wartości startowej dla semafora");
	}
			
	pid_t potomek = fork();
	int i;
	
	if(potomek == 0)
	{
		struct sembuf down_operation = {0, -1, 0};


		for(i = 0; i < 50; i++)
		{
			wait_and_increase_child(semafor_id);
			
			
			printf("Jestem procesem potomnym.\n");
			
			
			//odejmie wartość jeden od semafora 0 - pozwoli pętli rodzica kontynuować działanie
			if(semop(semafor_id, &down_operation, 1) < 0)
			{
				perror("P - Down nie poszlo");
			}
		}
	}
	else
	{
		struct sembuf down_operation = {1, -1, 0};


		for(i = 0; i < 50; i++)
		{			
			wait_and_increase_parent(semafor_id);
			
			
			printf("Jestem procesem rodzica.\n");
			
			
			//sprawdzanie, czy wartość jest większa od zera - bez tego zadziała nieprawidłowo
			if(semctl(semafor_id, 1, GETVAL) > 0)
			{
				//odejmie wartość 1 od semafora 1 - pozwoli pętli potomka kontynuować działanie
				if(semop(semafor_id, &down_operation, 1) < 0)
				{
					perror("R - Down nie poszlo");
				}
			}
		}
		
		//usuwa zbiór semaforów
		if(semctl(semafor_id, 0, IPC_RMID) < 0)
		{
			perror("Usuniecie nie poszlo");
		}
	}
	
	return 0;
}
