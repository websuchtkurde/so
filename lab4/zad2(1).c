#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	long mtype;
	char *mtext;
} w1, w2;

int funkcja(char *zdanie)
{
	int kolejka = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	pid_t potomek = fork();
	if(potomek == -1)
	perror("fork");
	
	if(potomek == 0)
	{
		w1.mtype = 1;
		w1.mtext = zdanie;

		if(msgsnd(kolejka, &w1, rozmiar, 0)<0)
		perror("msgsnd");
	}
	else
	{		
		if(msgrcv(kolejka, &w2, rozmiar, 0, 0)<0)
		perror("msgrcv");
		printf("Odebrana wiadomosc: %s\n", w2.mtext);
		
		if(msgctl(kolejka, IPC_RMID, 0)<0)
		perror("msgctl");
	}
	
	return 0;
}
int main(){
	char zdanie[30];
puts("jaka wiadomosc masz do przekazania? (ale nie dluzsza nic 30 znakow)");
scanf("%s",zdanie);
funkcja(zdanie);}

