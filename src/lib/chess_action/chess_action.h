//
// Created by elcoleoptero on 12/04/2021.
//

#ifndef CHES_CHESS_ACTION_H
#define CHES_CHESS_ACTION_H
#include "../chess_bitboard/chess_bitboard.h"
#include "../chess_display/chess_display.h"
#include <math.h>
typedef struct  BoardDescription BoardDescription;
struct BoardDescription
{
    uint64_t boards[2][6];
    int half_moves;
    uint8_t enPassant;
    uint8_t castling;
};

bool pawnValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int color);
uint64_t validerCoup(BoardDescription *boardDescription, char* position);
int8_t findPiece(BoardDescription *boardDescription, int indiceSrc,int color);
bool rookValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest);
bool bishopValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest);
bool queenValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest);
bool knightValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest);
bool kingValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest);
uint64_t colorAtacks(BoardDescription *boardDescription, int color);
bool isCheck(BoardDescription *boardDescription, int color);
void movePiece(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int piece, int color);
int killPiece(BoardDescription *boardDescription, int indiceDest,int color);
void resurrectPiece(BoardDescription *boardDescription, int indiceDest, int piece , int color);
bool kingCastling(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int color);

#endif //CHES_CHESS_ACTION_H
