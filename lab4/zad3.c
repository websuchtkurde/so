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

void funkcja(char *eftok)
{
	int id_kolejka = ftok(eftok, 0);
	if (id_kolejka <0)
	perror("ftok");
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	w1.mtype = 1;
	strcpy(w1.mtext, "jakis tekst");

	int wyslanie = msgsnd(kolejka, &w1, rozmiar, 0);

	if(wyslanie == -1)
	{
		perror("msgsnd");
	}
	else
	{
		printf("wiadomosc poszla do nadawcy!\n");
	}

	if(msgctl(kolejka, IPC_RMID, 0)<0)
	perror("msgctl");
	
	
}

int main(){
char eftok[20] = "zad3.c";
funkcja(eftok);
}
