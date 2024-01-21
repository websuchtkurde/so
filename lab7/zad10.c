#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


void *wykonaj_w1(void *data)
{
	//tutaj została wyłączona możliwość anulowania wątku
	if(pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL))
	fprintf(stderr,"Błąd przy linii 11");
	
	printf("(wątek 1) Komunikat z wątku 1 sprzed próby anulowania\n");
	
	sleep(2);
	
	//ten komunikat się wyświetli, bo pomimo próby jego anulowania podczas spania, 
	//wątek nie zostanie anulowany bo taka możliwość została wyłączona
	printf("(wątek 1) Komunikat z wątku 1 po próbie anulowania\n");
	
	return 0;
}

void *wykonaj_w2(void *data)
{
	//tutaj została włączona możliwość anulowania wątku - tak byłoby również bez tej linijki
	if(pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))
	fprintf(stderr,"Błąd przy linii 28");
	
	printf("(wątek 2) Komunikat z wątku 2 sprzed próby anulowania\n");
	
	sleep(2);
	
	//ten komunikat się nie wyświetli, bo w czasie gdy proces śpi to zostaje anulowany
	printf("(wątek 2) Komunikat z wątku 2 po próbie anulowania\n");
	
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
	
	sleep(1);
	
	if(pthread_cancel(watek1))
	fprintf(stderr,"Błąd linii 57");
	if(pthread_cancel(watek2))
	fprintf(stderr,"Błąd linii 59");
	
	if(pthread_join(watek1, NULL))
	fprintf(stderr,"Błąd linii 62");
	if(pthread_join(watek2, NULL))
	fprintf(stderr,"Błąd linii 62")
	;
	
	
	return 0;
}
