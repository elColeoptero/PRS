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
#include <stdbool.h>
#include <stdint.h>
#include "lib/read_lib/lire.h"
typedef struct  BoardDescription BoardDescription;
void displayShmInfo(int Id);
int nattch(int Id);
struct BoardDescription
{
    uint64_t boards[2][6];
    int half_moves;
    uint8_t enPassant;
    uint8_t rook;
};
void signalHandler(int signalNum) ;
int couleur;
sem_t *gameAccess;
pid_t* playersIndex;
BoardDescription* theGame;
int main(int argc, char const *argv[]){
	signal(SIGUSR1,signalHandler);	
	printf("mon pid : %d\n",getpid());
 	int shmid,shmid_partie;
	key_t key;
	key = ftok("/usr", 'R');

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
			shmid_partie = shmget (key+2, sizeof(BoardDescription) , 0666);
			theGame  =  shmat(shmid_partie, NULL, 0);
			gameAccess = sem_open ("gameAccess",0); 
			perror("CE pas ma fort");
			printf("Vous allez rejoindre la partie\n");
			couleur = 1;
			playersIndex[1]=getpid();
			kill(playersIndex[0],SIGUSR1);

			break;	
		}
		
	case 0 :
		playersIndex = shmat(shmid, NULL, 0);
		shmid_partie = shmget (key+2, sizeof(BoardDescription) , 0666 | IPC_CREAT);
		theGame  =  shmat(shmid_partie, NULL, 0);
		gameAccess = sem_open ("gameAccess", O_CREAT, 0666, 1); 
		printf("Creation partie\nVous êtes le premier joueur...\n");
		couleur = 0;
		playersIndex[0]=getpid();
		break;
		
	default : 
		printf("tchao bye bye\n");
		exit(-1);
	}
	while(1){

		pause();
	}
}

int nattch(int Id)
{
	struct shmid_ds info;
	shmctl(Id,IPC_STAT,&info);
	return info.shm_nattch;
}

void signalHandler(int signalNum) {

   bool jouerCoup=false;
   char srcMove[2], destMove[2];
   puts("d");
   sem_wait(gameAccess);
   printf("Afficher le bit bord \n\n");
   sem_post(gameAccess);
    
    sem_wait(gameAccess);
    printf("Test gameFInish \n\n");
    /*if(gameFinish())
    	exit(1);
    */
    sem_post(gameAccess);
    
    sem_wait(gameAccess);
    do{
	    printf("Jouez une coup plz : \n");
	    lire(srcMove,2);
	    lire(destMove,2);
	    jouerCoup=true;
	    //saisiUser();
    }while(!jouerCoup);
    sem_post(gameAccess);
    
    sleep(3);
    sem_wait(gameAccess);
    printf("Test gameFInish \n\n");
    /*if(gameFinish())
    	exit(1);
    */
    sem_post(gameAccess);
    kill(playersIndex[(couleur+1)%2],SIGUSR1);
}


