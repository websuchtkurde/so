#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>

struct msgbuf
{
	long mtype;
	char mtext[20];
} w1;
void wyslij(int kolejka, int rozmiar){
	char sendText[20];
			
			printf("\nNapisz wiadomosc do wyslania:");

			fgets(sendText, 20, stdin);
			
			w1.mtype = 1;
			
			char int_buf[20];	//tymczasowy bufor na liczbe
			sprintf(int_buf, "%ld", strlen(sendText));	//zmiana z long int na char*
			strcpy(w1.mtext, int_buf);	//dopisanie liczby do ciagu znakow
			
			int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
			if(wyslanie<0)
			perror("msgsnd");
			
			w1.mtype = 2;
			strcpy(w1.mtext, sendText);
			
			wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
				
			if(wyslanie == -1)
			{
				perror("Blad wysylania.");
			}
			else
			{
				printf("Wyslano wiadomosc.\n");
			}
}
void odbierz(int kolejka, int rozmiar){
int wiad = msgrcv(kolejka, &w1, rozmiar, 1, IPC_NOWAIT);
			
			long size = atoi(w1.mtext);
			
			printf("\nZa chwile odbierzesz wiadomosc o wielkosci: %ld\n", size);
		
			wiad = msgrcv(kolejka, &w1, rozmiar, 2, IPC_NOWAIT);
			
			if(wiad != -1)
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}

}
int main()
{
	int id_kolejka = ftok("zad8.c", 0);	
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(msgget<0)
			perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	while(1)
	{
		printf("\nWybierz opcje:");
		printf("\n1 - wyslanie komunikatu");
		printf("\n2 - odebranie komunikatu");
		printf("\n0 - zakoncz dzialanie programu\n");
		
		char buf[1];
		int choice = 0;
		
		fgets(buf, 4, stdin);
		sscanf(buf, "%d", &choice);
		
		if(choice == 1)
		{			
			wyslij(kolejka,rozmiar);
			
		}
		else if(choice == 2)
		{
			odbierz(kolejka,rozmiar);
		}
		else if(choice == 0)
		{
			break;
		}
		else
		{
			printf("\nWybrano nieprawidlowa opcje.");
		}
	}
	
	if(msgctl(kolejka, IPC_RMID, 0)<0)
	perror("msgctl");
	
	return 0;
}
