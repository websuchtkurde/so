#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


pthread_key_t klucz;


void *wykonaj_w1(void *data)
{	
	if(pthread_key_create(&klucz, NULL))
	fprintf(stderr,"Błąd w linii 13");
	
	int toSave = 1;
	
	if(pthread_setspecific(klucz, &toSave))
	fprintf(stderr,"Błąd w linii 18");
	
	printf("Wartość odczytana ze zmiennej prywatnej wątku 1: %d\n", *(int *)pthread_getspecific(klucz));
	
	return 0;
}

void *wykonaj_w2(void *data)
{
	if(pthread_key_create(&klucz, NULL))
	fprintf(stderr,"Error w linii 28");
	int toSave = 2;
	
	if(pthread_setspecific(klucz, &toSave))
	fprintf(stderr,"Błąd w linii 32");
	
	printf("Wartość odczytana ze zmiennej prywatnej wątku 2: %d\n", *(int *)pthread_getspecific(klucz));
	
	return 0;
}

int main(void)
{	
	pthread_t watek1, watek2;
	
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		fprintf(stderr,"Błąd tworzenia wątku 1");
	}
	
	if(pthread_create(&watek2, 0, wykonaj_w2, NULL))
	{
		fprintf(stderr,"Błąd tworzenia wątku 2");
	}
	
	if(pthread_join(watek1, NULL))
	fprintf(stderr,"Błąd w linii 54");
	if(pthread_join(watek2, NULL))
	fprintf(stderr,"Błąd w linii 56");
	
	if(pthread_cancel(watek1))
	fprintf(stderr,"Błąd w linii 59");
	if(pthread_cancel(watek2))
	fprintf(stderr,"Błąd w linii 61");
	
	return 0;
}
