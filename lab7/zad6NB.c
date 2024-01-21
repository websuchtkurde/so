#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>

static sigset_t maska;


void *wykonaj_w1(void *data)
{
	int kod_sygnalu;
	
	sigwait(&maska, &kod_sygnalu);
	
	printf("Otrzymano sygnał SIGALRM (kod: %d), więc można przejść dalej\n", kod_sygnalu);
	printf("(wątek 1)ID wątku: %ld\n", pthread_self());
	printf("(wątek 1)ID procesu: %d\n", getpid());
	
	return 0;
}

int main()
{
   	sigaddset (&maska, SIGALRM);	
	pthread_sigmask (SIG_BLOCK, &maska, NULL);
	
	pthread_t watek1;
	
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		perror("Błąd tworzenia wątku 1");
	}
	
	pthread_kill(watek1, SIGALRM);
	
	pthread_join(watek1, 0);
	
	return 0;
}
