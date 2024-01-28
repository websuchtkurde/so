#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
void funkcja(char *txt){
	int id = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	
	char *wspoldzielone = shmat(id, NULL, SHM_RDONLY);
	
	//przy próbie zapisu wyrzuci błąd, bo pamięć jest tylko do odczytu
	strcpy(wspoldzielone, txt);
	
	printf("Tekst odczytany z obszaru pamięci współdzielonej: %s\n", wspoldzielone);
	
	shmctl(id, IPC_RMID, 0);
}

int main(void)
{
	char txt[] = "tekst jakis";
	funkcja(txt);

}
