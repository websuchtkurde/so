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

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Nalezy podac jeden argument wywolania!\n");
		printf("1 - aby odczytac w kolejnosci rosnacej\n");
		printf("2 - aby odczytac w kolejnosci malejacej\n");
		
		return 0;
	}
	
	if(atoi(argv[1]) != 1 && atoi(argv[1]) != 2)
	{
		printf("Podano nieprawidlowy argument wywolania!\n");
		printf("1 - aby odczytac w kolejnosci rosnacej\n");
		printf("2 - aby odczytac w kolejnosci malejacej\n");
		
		return 0;
	}
	
	int id_kolejka = ftok("zad6.c", 0);
	if (id_kolejka<0)
	perror("ftok");
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("msgget");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	
	int i, wiad;
	
	if(atoi(argv[1]) == 1)
	{
		for(i = 1; i <= 10; i++)
		{
			wiad = msgrcv(kolejka, &w1, rozmiar, i, 0);
			if(wiad<0)
			perror("msgcrv");
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}
		}
	}
	else
	{
		for(i = 10; i > 0; i--)
		{
			wiad = msgrcv(kolejka, &w1, rozmiar, i, 0);
			if(wiad<0)
			perror("msgcrv");
		
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}
		}
	}
	
	
	if(msgctl(kolejka, IPC_RMID, 0)<0);
	perror("msgctl");
	
	return 0;
}
