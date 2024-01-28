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

int main(void)
{
	int id_kolejka = ftok("zad6.c", 0);	
	if (id_kolejka<0)
	perror("ftok");
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	int i;
	
	for(i = 1; i<= 10; i++)
	{
		w1.mtype = i;
		
		char int_buf[20];	//tymczasowy bufor na liczbe
		sprintf(int_buf, "%ld", w1.mtype);	//zmiana z long int na char*
		
		strcpy(w1.mtext, "jakis tekst nr ");
		strcat(w1.mtext, int_buf);	//dopisanie liczby do ciagu znakow
		
		int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
	
		if(wyslanie == -1)
		{
			perror("msgsnd");
		}
		else
		{
			printf("Wyslano wiadomosc nr: %ld\n", w1.mtype);
		}
	}
	
	if(msgctl(kolejka, IPC_RMID, 0)<0)
	perror("msgctl");
	
	return 0;
}
