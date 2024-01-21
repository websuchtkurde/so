#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

struct liczby
{
	int a, b;
} dane;


void *robotnik1(void *data)
{
	struct liczby *l = (struct liczby*) data;
	
	printf("Suma liczb: %d\n", l->a + l->b);

	return 0;
}

void *robotnik2(void *data)
{
	struct liczby *l = (struct liczby*) data;	
	
	printf("Różnica liczb: %d\n", l->a - l->b);

	return 0;
}

int main(void)
{
	pthread_t watek1, watek2;
	
	dane.a = 10;
	dane.b = 5;
	
	printf("Dane są liczby: %d i %d\n", dane.a, dane.b);
	
	if(pthread_create(&watek1, 0, robotnik1, (void *)&dane))
	{
		fprintf(stderr,"Blad przy tworzeniu watku %d\n",1);
	}
	
	if(pthread_create(&watek2, 0, robotnik2, (void *)&dane))
	{
		fprintf(stderr,"Blad przy tworzeniu watku %d\n",2);
	}
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Blad przy joinie watku %d\n",1);
	if(pthread_join(watek2, 0))
	fprintf(stderr,"Blad przy joinie watku %d\n",2);
	
	return 0;
}
