#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

//ten wątek jest łączny, więc proces główny sie skonczy dopiero jak sie zakonczy watek
void *wykonaj_w1(void *data)
{
	//gdyby tutaj zwiększyć sleep to oba wątki wypiszą tekst
	sleep(1);
	printf("(wątek 1)ID wątku: %ld\n", pthread_self());
	printf("(wątek 1)ID procesu: %d\n", getpid());
	
	return 0;
}

//wątek drugi jest rozdzielny, więc nic nie wypisze bo w momencie kiedy trwa sleep to sie watek glowny skonczy i ubije reszte co sie skonczyla lub byly rozdzielne
void *wykonaj_w2(void *data)
{
	sleep(1);
	printf("(wątek 2)ID wątku: %ld\n", pthread_self());
	printf("(wątek 2)ID procesu: %d\n", getpid());
	
	return 0;
}

int main(void)
{
	pthread_t watek1, watek2;
	
	pthread_attr_t attr;
	if(pthread_attr_init(&attr))
	fprintf(stderr,"Blad linii 34 %d\n",1);
	if(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
	fprintf(stderr,"Blad linii 36 %d\n",2);
	
	//laczny
	if(pthread_create(&watek1, 0, wykonaj_w1, NULL))
	{
		fprintf(stderr,"Blad tworzenia watku linia 40 %d\n",1);
	}
	
	//rozdzielny
	if(pthread_create(&watek2, &attr, wykonaj_w2, NULL))
	{
		fprintf(stderr,"Blad tworzenia watku linia 46 %d\n",2);
	}
	
	if(pthread_join(watek1, 0))
	fprintf(stderr,"Blad Joina watku linia 51 %d\n",1);
	if(pthread_join(watek2, 0))
	fprintf(stderr,"Blad Joina watku 53 %d\n",2);
	
	if(pthread_attr_destroy(&attr))
	fprintf(stderr,"Blad pthread attr 56 %d\n",2);
	
	return 0;
}
