#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>


void *wykonaj_w1(void *data)
{

	bool allowEnd = false;
	
	void obsluga(int a)
	{
		printf("Przyszedl sygnal SIGALRM\n");
		allowEnd = true;
	}
	
	struct sigaction new_action = {.sa_handler = obsluga};

	if(sigaction(SIGALRM, &new_action, NULL)<0)
	perror("sigaction");
	printf("(wątek 1)ID wątku: %ld\n", pthread_self());
	printf("(wątek 1)ID procesu: %d\n", getpid());
	
	while(!allowEnd);
	
	return 0;
}

int main(void)
{
	pthread_t watek1;
	
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		perror("Błąd tworzenia wątku 1");
	}
	
	sleep(1);
	
	if(pthread_kill(watek1, SIGALRM))
	fprintf(stderr,"Ubicie watku sie nie udalo - linia 43 %d\n",1);
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Blad joina watku linia 45 %d\n",1);
	
	return 0;
}
