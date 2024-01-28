#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf
{
	long mtype;
	char *mtext;
} w1, w2;

void funkcja(char *zdanie)
{
	int kolejka = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	w1.mtype = 1;
	
	w1.mtext = zdanie;
	
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);

	if(msgsnd(kolejka, &w1, rozmiar, 0)<0)
	perror("msgsnd");
	
	if(msgrcv(kolejka, &w2, rozmiar, 0, 0)<0)
	perror("msgrcv");
	else
	printf("Odebrana wiadomosc: %s\n", w2.mtext);
	
	if(msgctl(kolejka, IPC_RMID, 0)<0)
	perror("msgctl");
}
int main(){
	char zdanie[30];
puts("jaka wiadomosc masz do przekazania? (ale nie dluzsza nic 30 znakow)");
scanf("%s",zdanie);
funkcja(zdanie);

}