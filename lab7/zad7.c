#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>


void funkcjaSprzatajaca()
{
	printf("Napisala to F.sprzatajaca\n");
}

void *wykonaj_w1(void *data)
{
	printf("(wątek 1)ID wątku: %ld\n", pthread_self());
	printf("(wątek 1)ID procesu: %d\n", getpid());
	
	pthread_cleanup_push(funkcjaSprzatajaca, NULL);
	
	while(1)
	{
		pthread_testcancel();
		sleep(1);
	}
	
	pthread_cleanup_pop(NULL);
	
	return 0;
}

int main(void)
{	
	pthread_t watek1;
	
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		fprintf(stderr,"Blad tworzenia watku %d\n",2);
	}
	
	sleep(1);
	
	if(pthread_cancel(watek1))
	fprintf(stderr,"Blad w pthread_cancel - linia 44 %d\n",2);
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Blad przy pthread_join - linia 48 %d\n",2);
	
	return 0;
}
