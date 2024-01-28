#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

struct msgbuf
{
	long mtype;
	int mnumber;
} w1;

void funkcja()
{
	int id_kolejka = ftok("zad9a.c", 0);
	if(id_kolejka<0)
	perror("ftok");		
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	if(kolejka<0)
	perror("ftok");
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	int suma = 0;
	
	while(1)
	{
		int wiad = msgrcv(kolejka, &w1, rozmiar, 0, 0);
			
		if(wiad != -1)
		{
			printf("Odebrana liczba: %d\n", w1.mnumber);
			suma += w1.mnumber;
			
			printf("Suma wszystkich liczb: %d\n", suma);
		}
		else
		{
			break;
		}
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	if(msgctl<0)
	perror("msgctl");
	
	return 0;
}
int main(){
	int dummy;
	puts("wpisz cokolwiek i wcisnij enter jak chcesz zaczac");
	scanf("%d",dummy);
	funkcja();
}
