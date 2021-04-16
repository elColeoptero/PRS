/*******************************************************
Nom ......... : main.c
Role ........ : Permet de lancer ou de rejoindre une partie
Auteur ...... : POLAN

********************************************************/
#include "main.h"

//Global Var
sem_t *gameAccess;
InfoPlayers *thePlayers;
BoardDescription *theGame;

int main(int argc, char const *argv[])
{
    key_t key;
    key = ftok("/usr", 'R');
    newact.sa_handler = signalHandler;
    newact.sa_flags = 0;
    sigaction(SIGINT, &newact, NULL);
    sigaction(SIGUSR1, &newact, NULL);
    sigaction(SIGUSR2, &newact, NULL);
    system("clear");
    shmid = shmget(key, sizeof(InfoPlayers), 0666 | IPC_CREAT | IPC_EXCL);
    switch (errno)
    {
    case 17: //shm already exist
        shmid = shmget(key, sizeof(InfoPlayers), 0666);
        if ((nattch(shmid)) > 1)
        {
            printf("trop de joueur désolé\n");
            exit(1);
        }

        if ((nattch(shmid)) == 1)
        {
            initVar(1, key);
            if (argc > 1)
                strncpy(thePlayers->namePlayer[1], argv[1], 20);
            else
                strncpy(thePlayers->namePlayer[1], "Anonyme", 20);

            printf("Vous allez rejoindre la partie. \nMerci de patienter pour le premier coup de votre adversaire \n");
            kill(thePlayers->playersIndex[0], SIGUSR1);
            break;
        }

    case 0:
        printf("Creation partie.\nVous êtes le premier joueur veuillez attendre un adversaire...\n");
        initVar(0, key);
        if (argc > 1)
            strncpy(thePlayers->namePlayer[0], argv[1], 20);
        else
            strncpy(thePlayers->namePlayer[0], "Anonyme", 20);

        break;

    default:
        printf("Error\n");
        exit(-1);
    }
    while (1)
    {
        pause();
    }
}

/*******************************************************
Nom ......... : initVar
Role ........ : Permet d'initialiser les zones mémoires et le sémaphore
                pour les 2 joueurs
Arg ......... : color_conf : Couleur du joueur
                key        : Clé de la zone mémoire partagée
Return....... : VOID
********************************************************/

void initVar(int color_conf, int key)
{
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
    if (color_conf == 0)
        initBoard(theGame);
    else
    {
        init_bitboard();
    }
    thePlayers->playersIndex[color_conf] = getpid();
    color = color_conf;
}

/*******************************************************
Nom ......... : nattch
Role ........ : Permet de récupèrer le nombre de processus attaché à une zone mémoire
Arg ......... : Id : Id zone mémoire
Return....... : Nombre de processus attachés à cette zone mémoire
********************************************************/
int nattch(int Id)
{
    struct shmid_ds info;
    shmctl(Id, IPC_STAT, &info);
    return info.shm_nattch;
}

/*******************************************************
Nom ......... : signalHandler
Role ........ : En fonction du signal recu par le processus 
                lance un déroulement différent 
Arg ......... : Id : numero du signal recu
                    SIGINT : Fin de processus 
                    SIGUSR1: Jouer une coup et afficher le board
                    SIGUSR2: Fin du deuxieme joueur
Return....... : void
********************************************************/
void signalHandler(int signalNum)
{
    int returnjouerCoup = 0;
    char DestMove[3];
    char SrcMove[3];

    switch (signalNum)
    {
    case SIGINT:
        puts("\n\nVous abandonnez la partie. \nDefaite ! ");
        kill(thePlayers->playersIndex[(color + 1) % 2], SIGUSR2);
        sem_close(gameAccess);
        shmdt(thePlayers);
        shmdt(theGame);
        exit(1);
        break;

    case SIGUSR1:
        system("clear");
        sem_wait(gameAccess);
        displayGame(theGame, color, thePlayers->namePlayer);
        sem_post(gameAccess);

        sem_wait(gameAccess);
        if (
            isOver(theGame, color > 0))
        {
            sem_close(gameAccess);
            sem_unlink("gameAccess");
            shmdt(thePlayers);
            shmdt(theGame);
            shmctl(shmid_partie, IPC_RMID, NULL);
            exit(1);
        }

        sem_post(gameAccess);

        sem_wait(gameAccess);
        do
        {
            printf("Jouez une coup (entré pour continuer) ! \n source :\n");
            lire(SrcMove, 3);
            printf("\n destination  :\n");
            lire(DestMove, 3);
            returnjouerCoup = jouerCoup(theGame, SrcMove, DestMove, color);
            if (returnjouerCoup == 2)
                promotion(theGame, DestMove, color, 4);
        } while (returnjouerCoup == 0);
        sem_post(gameAccess);
        system("clear");
        sem_wait(gameAccess);
        displayGame(theGame, color, thePlayers->namePlayer);
        printf("is over : %d", isOver(theGame, color));
        if (isOver(theGame, (color + 1) % 2) > 0)
        {
            sem_close(gameAccess);
            shmdt(thePlayers);
            shmdt(theGame);
            exit(1);
        }

        sem_post(gameAccess);
        kill(thePlayers->playersIndex[(color + 1) % 2], SIGUSR1);
        puts("Votre adversaire joue...");
        break;
    case SIGUSR2:
        puts("\n\nVotre adversaire a décidé de quitter la partie vous gagnez par forfait. \nVictoire ! ");
        sem_close(gameAccess);
        sem_unlink("gameAccess");
        shmdt(thePlayers);
        shmctl(shmid, IPC_RMID, NULL);
        shmdt(theGame);
        shmctl(shmid_partie, IPC_RMID, NULL);
        exit(1);
        break;
    }
}
