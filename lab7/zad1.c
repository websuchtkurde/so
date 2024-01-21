#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void *robotnik1(void *data)
{
	printf("(wątek 1)id wątku: %ld\n", pthread_self());
	printf("(wątek 1)id procesu: %d\n", getpid());
	
	return 0;
}

void *robotnik2(void *data)
{
	printf("(wątek 2)id wątku: %ld\n", pthread_self());
	printf("(wątek 2)idprocesu: %d\n", getpid());
	
	return 0;
}

int main(void)
{
	pthread_t watek1, watek2;
	
	if(pthread_create(&watek1, 0, robotnik1, NULL))
	{
		fprintf(stderr,"Blad przy robieniu watku %d\n",1);
	}
	
	if(pthread_create(&watek2, 0, robotnik2, NULL))
	{
		fprintf(stderr,"Blad przy robieniu watku %d\n",1);

	}
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Blad przy laczeniu watku %d\n",1);;
	if(pthread_join(watek2, 0))
	fprintf(stderr,"Blad przy laczeniu watku %d\n",1);;
	
	return 0;
}
