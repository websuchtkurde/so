#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

void funkcja(char *txt)
{
	int id = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	if(id<0)
	perror("shmget");
	char *wspoldzielone = shmat(id, NULL, 0);
	
	strcpy(wspoldzielone, txt);
	
	printf("tekst odczytany z obszaru pamięci współdzielonej: %s\n", wspoldzielone);
	
	if(shmctl(id, IPC_RMID, 0)<0)
	perror("shmctl");
}
int main(void)
{
char txt[] = "tekst jakis";


funkcja(txt);
return 0;
}
