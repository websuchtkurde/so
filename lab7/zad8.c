#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static sigset_t maska;

void *wykonaj_w(void *data)
{
	int id_watku = *(int *)data;
	printf("(wątek %d)ID wątku: %ld\n", id_watku, pthread_self());
	
	if(pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	fprintf(stderr,"Blad przy linii 15 %d\n",2);
	
	//KILL nie ubija procesu tylko sygnal wysyla
	if(kill(getpid(), SIGINT)<0)
	perror("kill");
	printf("Zakończono wątek o id %ld\n", pthread_self());
	
	return 0;
}

int main()
{	
	if(sigaddset (&maska, SIGINT)<0)
	perror("sigaddset");	
	if(pthread_sigmask (SIG_BLOCK, &maska, NULL)<0)
	perror("pthread_sigmask");
	
	pthread_t watki[20];
	int i;
	
	for(i = 0; i < 20; i++)
	{
		//to dlatego że pamięć jest współdzielona z wątkami, więc jeśli przekażemy w parametrze i
		//to wątki nie odczytają "swojej" wartości tylko taką która akurat będzie największa - w
		//praktyce wszystkie wypisały by wartość 20
		int *a = malloc(sizeof(int));
		if(a==NULL)
		perror("malloc");
		*a = i;
		
		if(pthread_create(&watki[i], 0, wykonaj_w, a))
		{
			fprintf(stderr,"Błąd tworzenia wątku %d", i);
		}
		free(a);
	}

	int kod_sygnalu;
	sigwait(&maska, &kod_sygnalu);
	printf("\nKtóryś z wątków wysłał sygnał oznaczający zakończenie działania więc można przejść dalej i usunąć pozostałe wątki\n");
	
	for(i = 0; i < 20; i++)
	{
		if(pthread_cancel(watki[i]))
		fprintf(stderr,"Blad w linii 59 ");
	}
	
	printf("Usunięto pozostałe wątki asynchronicznie\n");
	
	return 0;
}
