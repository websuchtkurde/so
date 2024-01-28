#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

struct msgbuf
{
	long mtype;
	char mtext[20];
} w1;

int main()
{
	int id_kolejka = ftok("zad5.c", 0);	
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	int i;
	srand(time(0));
	
	for(i = 1; i<= 5; i++)
	{
		w1.mtype = rand()%4 + 1;
		
		char tekst[20] = "jakis tekst nr ";
		char typ = w1.mtype + '0';
		
		strcat(tekst, &typ);
		strcpy(w1.mtext, tekst);
		
		int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
	
		if(wyslanie == -1)
		{
			perror("Blad wysylania.");
		}
		else
		{
			printf("Wyslano wiadomosc nr: %ld\n", w1.mtype);
		}
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	
	return 0;
}
