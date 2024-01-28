#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <wait.h>
#include <time.h>

void print_value(int semafor_id)
{
	int i;
	for(i = 0; i < 5; i++)
	{
		printf("Wartosc semafora %d wynosi: %d\n", i, semctl(semafor_id, i, GETVAL));
	}
}


int main()
{
	int semafor_id = semget(IPC_PRIVATE, 5, 0600 | IPC_CREAT);
	int i;

	if(semafor_id < 0)
	{
		perror("Nie udalo sie utworzyć semaforów");
	}
	
	printf("Wartości semaforów przed użyciem SETALL:\n");
	print_value(semafor_id);
	
	//tablica z zapisanymi wartościami dla semaforów
	unsigned short arg[5];
	for(i = 0; i < 5; i++)
	{
		arg[i] = i*3+1; //to po prawej dowolne!
	}
	
	//przydzielanie wartości semaforom
	if(semctl(semafor_id, 0, SETALL, arg) < 0)
	{
		perror("Nie udalo sie przydzielic wartosci semaforom\n");
	}

	printf("\nWartości semaforów po użyciu SETALL:\n");
	print_value(semafor_id);
	
	//utworzenie pustej tablicy (wraz z przydzieleniem pamięci) do której zostaną pobrane wartości z semaforów
	unsigned short *arg2 = (unsigned short *)calloc(5, sizeof(unsigned short));
	
	//pobranie wartości semaforom
	if(semctl(semafor_id, 0, GETALL, arg2) < 0)
	{
		perror("Nie udalo sie pobrać wartosci semaforów\n");
	}

	printf("\nWartości semaforów pobrane z użyciem GETALL:\n");
	for(i = 0; i < 5; i++)
	{
		printf("Wartość semafora %d wynosi: %d\n", i, arg2[i]);
	}
	
	//struktura do której zostaną wczytane informacje o semaforach pobrane przez IPC_STAT
	struct semid_ds buf;
	
	//pobranie informacji o semaforach
	if(semctl(semafor_id, 0, IPC_STAT, &buf) < 0)
	{
		perror("Nie udalo sie pobrać informacji o semaforach\n");
	}
	
	
	//wyświetlenie części informacji statystycznych o semaforach - do zadania raczej niepotrzebne, albo wyświetlić samo sem_nsems
	printf("\nIlość semaforów: %ld\n", buf.sem_nsems);
	printf("Data wykonania ostatniej operacji semop na zbiorze semaforów: %s\n", ctime(&buf.sem_otime));
	printf("Data wykonania ostatniej operacji semctl na zbiorze semaforów: %s\n", ctime(&buf.sem_ctime));
	
	
	if(semctl(semafor_id, 0, IPC_RMID) < 0)
	{
		perror("Usuniecie nie poszlo");
	}


	return 0;
}
