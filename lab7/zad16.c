#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <wait.h>

int liczba = -1;
int dzielniki = 2;	//zakładam że każda jest podzielna przez 1 i przez samą siebie

pthread_mutex_t mutex1;
pthread_cond_t waitForNewValue;

struct sembuf wait_operation = {0, -5, 0};
struct sembuf up_operation = {0, 1, 0};


void *farmer(void *semafor_id)
{
	int i;
	
	for(i = 0; i < 32; i++)
	{
		liczba = i;
		
		if(i == 0)
		{
			sleep(1);
		}
		
		if(pthread_cond_broadcast(&waitForNewValue))
		fprintf(stderr,"Błąd w linii 33");
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &wait_operation, 1)<0)
		perror("semop");
		
		if(dzielniki == 2)
		{
			printf("Liczba %d jest liczbą pierwszą\n", liczba);
		}
		else
		{
			printf("Liczba %d nie jest liczbą pierwszą\n", liczba);
		}
		
		dzielniki = 2;
	}
	
	return 0;
}

void *r1(void *semafor_id)
{
	int moja_liczba = 2;
	
	while(liczba < 32)
	{
		if(pthread_cond_wait(&waitForNewValue, &mutex1))
		fprintf(stderr,"Błąd w linii 61");
		
		if(liczba % moja_liczba == 0 && liczba != moja_liczba)
		{
			dzielniki++;
		}
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &up_operation, 1)<0)
		perror("semop");
	}
	
	return 0;
}

void *r2(void *semafor_id)
{
	int moja_liczba = 3;
	
	while(liczba < 32)
	{
		if(pthread_cond_wait(&waitForNewValue, &mutex1))
		fprintf(stderr,"Błąd w linii 83");
		
		if(liczba % moja_liczba == 0 && liczba != moja_liczba)
		{
			dzielniki++;
		}
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &up_operation, 1)<0)
		perror("semop");
	}
	
	return 0;
}

void *r3(void *semafor_id)
{
	int moja_liczba = 5;
	
	while(liczba < 32)
	{
		if(pthread_cond_wait(&waitForNewValue, &mutex1))
		fprintf(stderr,"Błąd w linii 105");
		
		if(liczba % moja_liczba == 0 && liczba != moja_liczba)
		{
			dzielniki++;
		}
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &up_operation, 1)<0)
		perror("semop");
	}
	
	return 0;
}

void *r4(void *semafor_id)
{
	int moja_liczba = 7;
	
	while(liczba < 32)
	{
		if(pthread_cond_wait(&waitForNewValue, &mutex1))
		fprintf(stderr,"Błąd w linii 125");
		
		if(liczba % moja_liczba == 0 && liczba != moja_liczba)
		{
			dzielniki++;
		}
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &up_operation, 1)<0)
		perror("semop");
	}
	
	return 0;
}

void *r5(void *semafor_id)
{
	int moja_liczba = 11;
	
	while(liczba < 32)
	{
		if(pthread_cond_wait(&waitForNewValue, &mutex1))
		fprintf(stderr,"Błąd w linii 146");
		
		if(liczba % moja_liczba == 0 && liczba != moja_liczba)
		{
			dzielniki++;
		}
		
		int *id_sem = (int *)semafor_id;
		if(semop(*id_sem, &up_operation, 1)<0)
		perror("semop");
	}
	
	return 0;
}

int main()
{
	int semafor_id = semget(IPC_PRIVATE, 1, 0600 | IPC_CREAT);
	if(semafor_id<0)
	perror("semget");
	if(pthread_mutex_init(&mutex1, NULL))
	fprintf(stderr,"Błąd w linii 170");
    if(pthread_cond_init(&waitForNewValue, NULL)){
	fprintf(stderr,"Błąd w linii 172");
	}
    
	pthread_t w_farmer, w_r1, w_r2, w_r3, w_r4, w_r5;
	
	if(pthread_create(&w_farmer, 0, farmer, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu farmera");
	}
	
	if(pthread_create(&w_r1, 0, r1, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu robotnika 1");
	}
	
	if(pthread_create(&w_r2, 0, r2, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu robotnika 2");
	}
	
	if(pthread_create(&w_r3, 0, r3, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu robotnika 3");
	}
	
	if(pthread_create(&w_r4, 0, r4, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu robotnika 4");
	}
	
	if(pthread_create(&w_r5, 0, r5, (void *)&semafor_id))
	{
		fprintf(stderr,"Błąd w robieniu robotnika 5");
	}
	
	if(pthread_join(w_farmer, 0))
	fprintf(stderr,"Error -  thread join farmer");
	if(pthread_join(w_r1, 0))
	fprintf(stderr,"Error -  thread join robotnik 1");
	if(pthread_join(w_r2, 0))
	fprintf(stderr,"Error -  thread join robotnik 2");
	if(pthread_join(w_r3, 0))
	fprintf(stderr,"Error -  thread join robotnik 3");
	if(pthread_join(w_r4, 0))
	fprintf(stderr,"Error -  thread join robotnik 4");
	if(pthread_join(w_r5, 0))
	fprintf(stderr,"Error -  thread join robotnik 5");
	
	if(semctl(semafor_id, 0, IPC_RMID) < 0)
	{
		perror("semctl");
	}
	
	if(pthread_mutex_destroy(&mutex1))
	fprintf(stderr,"Blad przy niszczeniu mutexa");
    	if(pthread_cond_destroy(&waitForNewValue))
		fprintf(stderr,"Blad przy niszczeniu zmiennej warunkowej");
	
	return 0;
}

// 1 wyjdzie jako liczba pierwsza - to wynika z założeń w poleceniu
// można poprawić żeby program się zakańczał
