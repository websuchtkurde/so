#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


pthread_key_t klucz1;
pthread_key_t klucz2;


void *wykonaj_w1(void *data)
{	
	if(pthread_key_create(&klucz2, NULL))
	fprintf(stderr,"Błąd w linii 14");
	
	int toSave = 1;
	
	if(pthread_setspecific(klucz2, &toSave))
	fprintf(stderr,"Błąd w linii 19");
	
	sleep(1);
	//zmienic ponizej za klucz 2 i vice versa to wywali seg-fault
	printf("Wartość odczytana ze zmiennej prywatnej wątku 2: %d\n", *(int *)pthread_getspecific(klucz2));
	
	return 0;
}

void *wykonaj_w2(void *data)
{
	if(pthread_key_create(&klucz1, NULL))
	fprintf(stderr,"Błąd - linia 31");
	
	int toSave = 2;
	
	if(pthread_setspecific(klucz1, &toSave))
	fprintf(stderr,"Błąd - linia 36");
	
	sleep(1);
	
	printf("Wartość odczytana ze zmiennej prywatnej wątku 1: %d\n", *(int *)pthread_getspecific(klucz1));
	
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
	fprintf(stderr,"Błąd w linii 60");
	if(pthread_join(watek2, NULL))
	fprintf(stderr,"Błąd w linii 62");
	
	if(pthread_cancel(watek1))
	fprintf(stderr,"Błąd w linii 65");
	if(pthread_cancel(watek2))
	fprintf(stderr,"Error w linii 67");
	
	return 0;
}
