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
#include "lib/read_lib/read_lib.h"
#include "lib/chess_game/chess_game.h"
#include <stdlib.h>

typedef struct InfoPlayers InfoPlayers;
struct InfoPlayers
{
    char namePlayer[2][20];
    pid_t playersIndex[2];
};
struct sigaction newact;
void displayShmInfo(int Id);
int nattch(int Id);
void signalHandler(int signalNum);
void initVar(int color_conf, int key);
int shmid_partie, shmid;
int color;
