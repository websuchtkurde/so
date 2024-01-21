#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

struct liczby
{
	int a, b;
} dane;


void *wykonaj_w1(void *data)
{
	struct liczby *l = (struct liczby*) data;
	
	int value = l->a + l->b;
	int *wynik = malloc(sizeof(int));
	if(wynik==NULL)
	perror("malloc");
	*wynik = value;

	return (void *)wynik;
}

void *wykonaj_w2(void *data)
{
	struct liczby *l = (struct liczby*) data;	
	
	int value = l->a - l->b;
	int *wynik = malloc(sizeof(int));
	if(wynik==NULL)
	perror("malloc");
	
	*wynik = value;

	return (void *)wynik;
}

int main(void)
{
	pthread_t watek1, watek2;
	
	dane.a = 10;
	dane.b = 5;
	
	int *status;
	
	printf("Dane są liczby: %d i %d\n", dane.a, dane.b);
	
	if(pthread_create(&watek1, 0, wykonaj_w1, (void *)&dane))
	{
		fprintf(stderr,"Blad tworzenia watku%d\n",1);
	}
	
	if(pthread_create(&watek2, 0, wykonaj_w2, (void *)&dane))
	{
		fprintf(stderr,"Blad tworzenia watku %d\n",2);
	}
	
	if(pthread_join(watek1, (void **)&status))
	fprintf(stderr,"Blad joina watku %d\n",1);
	printf("Suma liczb: %d\n", *status);
	
	if(pthread_join(watek2, (void **)&status))
	fprintf(stderr,"Blad joina watku %d\n",2);
	printf("Różnica liczb: %d\n", *status);
	
	return 0;
}
