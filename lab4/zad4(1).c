#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf
{
	long mtype;
	char mtext[20];
} w1;

int main()
{
	int id_kolejka = ftok("zad4.c", 0);
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	w1.mtype = 1;
	strcpy(w1.mtext, "jakis tekst");

	int wyslanie = msgsnd(kolejka, &w1, rozmiar, 0);
	
	if(wyslanie == -1)
	{
		perror("Blad wysylania.");
	}
	else
	{
		printf("Wyslano wiadomosc.\n");
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	
	return 0;
}

