#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

struct msgbuf
{
	long mtype;
	int mnumber;
} w1;

int main()
{
	int id_kolejka = ftok("zad9a.c", 0);
	if(id_kolejka<0)
	perror("ftok");		
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("ftok");	
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	int i;
	
	for(i = 1; i<100; i+=2)
	{
		w1.mtype = 2;
		w1.mnumber = i;
		
		int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
		
		if(wyslanie == -1)
		{
			perror("Blad wysylania.");
		}
		else
		{
			printf("Wyslano liczbe: %d\n", w1.mnumber);
		}
		
		sleep(1);
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	if(msgctl<0)
	perror("msgctl");	
	
	return 0;
}
