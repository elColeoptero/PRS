#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wait.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include "lib/read_lib/lire.h"
#include "lib/chess_display/display_chess.h"
#include <stdlib.h>
// TEMP
typedef struct BoardDescription BoardDescription;
struct BoardDescription
{
    uint64_t boards[2][6];
    int half_moves;
    uint8_t enPassant;
    uint8_t rook;
};

typedef struct InfoPlayers InfoPlayers;
struct InfoPlayers
{
    char namePlayer[2][20];
    pid_t playersIndex[2];
};
struct sigaction newact;
// .h
void displayShmInfo(int Id);
int nattch(int Id);
void signalHandler(int signalNum);
void initVar(int color_conf, int shmid, int key);
int color;
sem_t *gameAccess;
InfoPlayers *thePlayers;
BoardDescription *theGame;
int main(int argc, char const *argv[])
{
    int shmid;
    key_t key;
    key = ftok("/usr", 'R');
    newact.sa_handler = signalHandler;
    newact.sa_flags = 0;
    sigaction(SIGINT, &newact, NULL);
    sigaction(SIGUSR1, &newact, NULL);
    sigaction(SIGUSR2, &newact, NULL);
    system("clear");
    shmid = shmget(key, 2 * sizeof(pid_t), 0666 | IPC_CREAT | IPC_EXCL);
    switch (errno)
    {
    case 17:
        shmid = shmget(key, 2 * sizeof(pid_t), 0666);
        if ((nattch(shmid)) > 1)
        {
            printf("trop de joueur déso\n");
            exit(1);
        }

        if ((nattch(shmid)) == 1)
        {
            initVar(1, shmid, key);
            if (argc > 1)
                strncpy(thePlayers->namePlayer[1], argv[1], 20);
            else
                strncpy(thePlayers->namePlayer[1], "Annonyme", 20);

            printf("Vous allez rejoindre la partie. \nMerci de patienter pour le premier coup de votre adversaire \n");
            kill(thePlayers->playersIndex[0], SIGUSR1);
            break;
        }

    case 0:
        printf("Creation partie. Vous êtes le premier joueur...\n");
        initVar(0, shmid, key);
        if (argc > 1)
            strncpy(thePlayers->namePlayer[0], argv[1], 20);
        else
            strncpy(thePlayers->namePlayer[0], "Annonyme", 20);

        break;

    default:
        printf("tchao bye bye\n");
        exit(-1);
    }
    while (1)
    {
        pause();
    }
}

void initVar(int color_conf, int shmid, int key)
{
    int shmid_partie;
    thePlayers = shmat(shmid, NULL, 0);
    if (color_conf == 0)
    {
        shmid_partie = shmget(key + 2, sizeof(BoardDescription), 0666 | IPC_CREAT);
        gameAccess = sem_open("gameAccess", O_CREAT, 0666, 1);
    }
    else
    {
        shmid_partie = shmget(key + 2, sizeof(BoardDescription), 0666);
        gameAccess = sem_open("gameAccess", 0);
    }
    theGame = shmat(shmid_partie, NULL, 0);
    thePlayers->playersIndex[color_conf] = getpid();
    color = color_conf;
}

int nattch(int Id)
{
    struct shmid_ds info;
    shmctl(Id, IPC_STAT, &info);
    return info.shm_nattch;
}

void signalHandler(int signalNum)
{
    bool jouerCoup = false;
    char DestMove[5];
    char SrcMove[5];

    switch (signalNum)
    {
    case SIGINT:
        puts("\n\nVous abandonnez la partie. \nDefaite ! ");
        kill(thePlayers->playersIndex[(color + 1) % 2], SIGUSR2);
        sem_close(gameAccess);
        exit(1);
        break;

    case SIGUSR1:
        system("clear");
        sem_wait(gameAccess);
        int board_array[] = {3, 1, 2, 4, 5, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 6, 6, 6, 6, 6, 6, 6, 6, 9, 7, 8, 10, 11, 8, 7, 9};
        displayBoard(board_array, color, thePlayers->namePlayer);
        sem_post(gameAccess);

        sem_wait(gameAccess);
        /*if(gameFinish())
	    	exit(1);
	    */
        sem_post(gameAccess);

        sem_wait(gameAccess);
        do
        {
            printf("Jouez une coup sous la forme : source-destination \n");
            lire(SrcMove, 5);
            lire(DestMove, 5);
            jouerCoup = true;
            //saisiUser();
        } while (!jouerCoup);
        sem_post(gameAccess);
        system("clear");
        sem_wait(gameAccess);
        int board_array2[] = {3, 1, 2, 4, 5, 2, 1, 3, 12, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 6, 6, 6, 6, 6, 6, 6, 6, 9, 7, 8, 10, 11, 8, 7, 9};
        displayBoard(board_array2, color, thePlayers->namePlayer);
        /*if(gameFinish())
	    	exit(1);
	    */
        sem_post(gameAccess);
        kill(thePlayers->playersIndex[(color + 1) % 2], SIGUSR1);
        puts("Votre adversaire joue...");
        break;
    case SIGUSR2:
        puts("\n\nVotre adversaire a décidé de quitter la partie vous gagnez par forfait. \nVictoire ! ");
        sem_close(gameAccess);
        sem_unlink("gameAccess");
        exit(1);
        break;
    }
}