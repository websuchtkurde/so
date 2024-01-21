#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>


void *wykonaj_w1(void *data)
{
	pthread_attr_t attr;
	if(pthread_attr_init(&attr))
	fprintf(stderr,"Błąd w linii 12");
	int i;
	
	if(pthread_attr_getdetachstate(&attr, &i))
	fprintf(stderr,"Błąd w linii 15");
	if(i == PTHREAD_CREATE_DETACHED)
	{
		printf("Wątek jest rozdzielny\n");
	}
	else
	{
		printf("Wątek jest łączny\n");		
	}
	
	if(pthread_attr_getscope(&attr, &i))
	fprintf(stderr,"Błąd w linii 25");
	if(i == PTHREAD_SCOPE_PROCESS)
	{
		printf("Wątek współdzieli zasoby wraz z innymi wątkami w obrębie procesu\n");
	}
	else if(i == PTHREAD_SCOPE_SYSTEM)
	{
		printf("Wątek współdzieli zasoby wraz z innymi procesami w systemie\n");
	}
	
	if(pthread_attr_getinheritsched(&attr, &i))
	fprintf(stderr,"Błąd w linii 35");
	if(i == PTHREAD_INHERIT_SCHED)
	{
		printf("Wątek dziedziczy atrybuty z wątku tworzącego\n");
	}
	else if(i == PTHREAD_EXPLICIT_SCHED)
	{
		printf("Wątek pobiera atrybuty z wartości określonych przez obiekt atrybutów\n");
	}
	
	if(pthread_attr_destroy(&attr))
	fprintf(stderr,"Błąd w linii 45");
		
	return 0;
}

int main(void)
{
	pthread_t watek1;
	
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		fprintf(stderr,"Błąd przy tworzeniu watku 1");
	}
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Błąd w linii 59");
	
	return 0;
}
