#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
void displayShmInfo(int Id);
int nattch(int Id);
void signalHandler(int signalNum) ;
int couleur;
sem_t *gameAccess;

int main(int argc, char const *argv[]){
	signal(SIGUSR1,signalHandler);	
	printf("mon pid : %d",getpid());
 	int shmid;
	key_t key;
	key = ftok("/usr", 'R');
	pid_t* playersIndex;
	shmid = shmget (key, 2*sizeof(pid_t) , 0666 | IPC_CREAT | IPC_EXCL);
	int errsv = errno;
	switch(errsv){
	case 17 :
		shmid = shmget (key, 2*sizeof(pid_t) , 0666 );
		if((nattch(shmid))>1)
		{
			printf("trop de joueur déso\n");
			exit(1);
		}
		
		if((nattch(shmid))==1)
		{
			playersIndex = shmat(shmid, NULL, 0);
			printf("Vous allez rejoindre la partie\n");
			couleur = 1;
			playersIndex[1]=getpid();
			gameAccess = sem_open ("gameAccess", O_CREAT, 0666, 1); 
			sem_unlink ("gameAccess"); 
			printf("J'inite la partie hehe\n");
			printf("J'affiche la partie hehe\n");
			kill(playersIndex[0],SIGUSR1);
			printf("%d",playersIndex[0]);
			break;	
		}
		
	case 0 :
		playersIndex = shmat(shmid, NULL, 0);
		printf("Creation partie\nVous êtes le premier joueur...\n");
		couleur = 0;
		playersIndex[0]=getpid();
		break;
		
	default : 
		printf("tchao bye bye\n");
		exit(-1);
	}
	
	//while(1){
		pause();
		printf("COUCOU SAMUEL");
	//}
}

int nattch(int Id)
{

	struct shmid_ds info;
	shmctl(Id,IPC_STAT,&info);
	return info.shm_nattch;
}

void signalHandler(int signalNum) {
    printf("Caught SIGUSR1 signal.\n");
    pause();
}


