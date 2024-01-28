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
void wyslij(int kolejka, int rozmiar){
printf("\nNapisz wiadomosc do wyslania:");
			fgets(w1.mtext, 20, stdin);
			
			printf("\nWybierz id komunikatora do ktorego chcesz wyslac wiadomosc:");
			
			char buf[4];
			int choice_id = 0;
			fgets(buf, 4, stdin);
			sscanf(buf, "%d", &choice_id);
			
			if(choice_id < 1 || choice_id > 3)
			{
				printf("\nWybrano nieprawidlowe id komunikatora\n");
			}
			else
			{
				w1.mtype = choice_id;
				int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
				
				if(wyslanie == -1)
				{
					perror("Blad wysylania.");
				}
				else
				{
					printf("Wyslano wiadomosc do komunikatora nr %ld\n", w1.mtype);
				}
			}
}
void odbierz(int kolejka, int rozmiar, int kom_id){
    int wiad = msgrcv(kolejka, &w1, rozmiar, kom_id, IPC_NOWAIT);
			
			if(wiad != -1)
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}

}
int main()
{
	int id_kolejka = ftok("zad7a.c", 0);	
	
	int kolejka = msgget(id_kolejka, IPC_CREAT | 0666);
	int rozmiar = sizeof(struct msgbuf) - sizeof(w1.mtype);
	int kom_id = 2;
	
	printf("ID komunikatora: %d", kom_id);
	
	while(1)
	{
		printf("\nWybierz opcje:");
		printf("\n1 - wyslanie komunikatu");
		printf("\n2 - odebranie komunikatu");
		printf("\n0 - zakoncz dzialanie programu\n");
		
		char buf[4];
		int choice = 0;
		
		fgets(buf, 4, stdin);
		sscanf(buf, "%d", &choice);
		
		if(choice == 1)
		{		wyslij(kolejka, rozmiar);
			/*printf("\nNapisz wiadomosc do wyslania:");
			fgets(w1.mtext, 20, stdin);
			
			printf("\nWybierz id komunikatora do ktorego chcesz wyslac wiadomosc:");
			
			char buf[1];
			int choice_id = 0;
			fgets(buf, 4, stdin);
			sscanf(buf, "%d", &choice_id);
			
			if(choice_id < 1 || choice_id > 3)
			{
				printf("\nWybrano nieprawidlowe id komunikatora\n");
			}
			else
			{
				w1.mtype = choice_id;
				int wyslanie = msgsnd(kolejka, &w1, rozmiar, IPC_NOWAIT);
				
				if(wyslanie == -1)
				{
					perror("Blad wysylania.");
				}
				else
				{
					printf("Wyslano wiadomosc do komunikatora nr %ld\n", w1.mtype);
				}
			}*/
		}
		else if(choice == 2)
		{
            odbierz(kolejka,rozmiar,kom_id);
            /*
			int wiad = msgrcv(kolejka, &w1, rozmiar, kom_id, IPC_NOWAIT);
			
			if(wiad != -1)
			{
				printf("Odebrana wiadomosc: %s\n", w1.mtext);
			}*/
		}
		else if(choice == 0)
		{
			break;
		}
		else
		{
			printf("\nWybrano nieprawidlowa opcje.");
		}
	}
	
	msgctl(kolejka, IPC_RMID, 0);
	
	return 0;
}
