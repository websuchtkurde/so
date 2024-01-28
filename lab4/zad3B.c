#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	long mtype;
	char mtext[20];
} w1;

void funkcja(char *eftok)
{
	int id_kolejka = ftok("zad3.c", 0);
	if (id_kolejka<0)
	perror("ftok");
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
		
	if(msgrcv(kolejka, &w1, rozmiar, 0, IPC_NOWAIT)<0);
	printf("Odebrana wiadomosc: %s\n", w1.mtext);
		
	if(msgctl(kolejka, IPC_RMID, 0)<0)
	perror("msgctl");
	
}

int main(){
char eftok[20] = "zad3.c";
funkcja(eftok);
}
