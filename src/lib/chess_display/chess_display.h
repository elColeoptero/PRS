//
// Created by elcoleoptero on 30/03/2021.
//

#ifndef CHES_CHESS_DISPLAY_H
#define CHES_CHESS_DISPLAY_H
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
void displayBoard(int board_array[], int couleurJoueur, char nameJoueur[2][20]);
void bitBoardToArray(uint64_t boards[2][6], int tab[]);
void bb_to_str(uint64_t b1);
void bb_to_str_bis(uint64_t b1, uint64_t b2, uint64_t b3);

#endif //CHES_CHESS_DISPLAY_H
