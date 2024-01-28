#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>



void funkcja()
{
	struct shmid_ds bufor;
	int id = shmget(IPC_PRIVATE, 50, IPC_CREAT | 0600);
	
	shmctl(id, IPC_STAT, &bufor);
	printf("PID procesu, który stworzył: %o\n", bufor.shm_cpid);
	printf("Prawa dostępu: %o\n", bufor.shm_perm.mode);
	printf("Ilość podłączeń (aktualnych): %ld\n", bufor.shm_nattch);
	printf("Rozmiar: %lo\n", bufor.shm_segsz);
	
	shmctl(id, IPC_RMID, 0);
}
int main(void)
{

	funkcja();
}