#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

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
	
	int i, wiad = -1;
	
	for(i = 1; i <= 5; i++)
	{
		do
		{
			wiad = msgrcv(kolejka, &w1, rozmiar, i,0);
			
			if(wiad != -1)
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}
				
		} while(wiad != -1);
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	
	return 0;
}
