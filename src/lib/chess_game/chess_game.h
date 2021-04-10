//
// Created by elcoleoptero on 30/03/2021.
//

#ifndef CHES_CHESS_GAME_H
#define CHES_CHESS_GAME_H
#include "bitboard.h"
#include "display_chess.h"
#include <math.h>
typedef struct  BoardDescription BoardDescription;
struct BoardDescription
{
    uint64_t boards[2][6];
    int half_moves;
    uint8_t enPassant;
    uint8_t rook;
};
void initBoard();
void displayGame(int player);
bool jouerCoup(char* src, char* dest);
uint64_t validerCoup(char* position);
int8_t findPiece(int indiceSrc);
bool rookValidation(int indiceSrc, int indiceDest);
uint64_t colorAtacks(int color);
bool isCheck(int color);
void pieceMouving(int indiceSrc, int indiceDest, int piece, int color);

#endif //CHES_CHESS_GAME_H
