//
// Created by elcoleoptero on 30/03/2021.
//

#ifndef CHES_CHESS_GAME_H
#define CHES_CHESS_GAME_H

#include "../chess_display/chess_display.h"
#include "../chess_action/chess_action.h"
#include <math.h>

void initBoard(BoardDescription *boardDescription );
void displayGame(BoardDescription *boardDescription,int color, char nameJoueur[2][20]);
int jouerCoup(BoardDescription *boardDescription, char *positionSrc, char *positionDest, int color);
bool promotion(BoardDescription *boardDescription, char *positionDest, int color, int piece);

int verifCoup(BoardDescription *boardDescription,int indiceSrc, int indiceDests, int color);
int isOver(BoardDescription *boardDescription,int color);
bool stalemate(BoardDescription *boardDescription,int color);
#endif //CHES_CHESS_GAME_H
