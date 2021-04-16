//
// Created by elcoleoptero on 30/03/2021.
//

#include "chess_game.h"
#include "../chess_bitboard/chess_bitboard.h"


// TODO : put pointer to boardDescription
void initBoard(BoardDescription *boardDescription ) {

    boardDescription->boards[0][0] = 0b0000000000000000000000000000000000000000000000001111111100000000;
    boardDescription->boards[0][1] = 0b0000000000000000000000000000000000000000000000000000000001000010;
    boardDescription->boards[0][2] = 0b0000000000000000000000000000000000000000000000000000000000100100;
    boardDescription->boards[0][3] = 0b0000000000000000000000000000000000000000000000000000000010000001;
    boardDescription->boards[0][4] = 0b0000000000000000000000000000000000000000000000000000000000001000;
    boardDescription->boards[0][5] = 0b0000000000000000000000000000000000000000000000000000000000010000;
    boardDescription->boards[1][0] = 0b0000000011111111000000000000000000000000000000000000000000000000;
    boardDescription->boards[1][1] = 0b0100001000000000000000000000000000000000000000000000000000000000;
    boardDescription->boards[1][2] = 0b0010010000000000000000000000000000000000000000000000000000000000;
    boardDescription->boards[1][3] = 0b1000000100000000000000000000000000000000000000000000000000000000;
    boardDescription->boards[1][4] = 0b0000100000000000000000000000000000000000000000000000000000000000;
    boardDescription->boards[1][5] = 0b0001000000000000000000000000000000000000000000000000000000000000;
    boardDescription->enPassant = 64; // no enPassant allowed
    boardDescription->half_moves = 0;
    boardDescription->castling = 0b00001111; // 4 rocks are possible

    init_bitboard();

}

void displayGame(BoardDescription *boardDescription,int color,char nameJoueur[2][20]) {
    int tab[64];
    bitBoardToArray(boardDescription->boards, tab);
    displayBoard(tab,color,nameJoueur);
}

int jouerCoup(BoardDescription *boardDescription,char *positionSrc, char *positionDest, int color) {
    int  indiceSrc = validerCoup(boardDescription,positionSrc);
    int  indiceDest = validerCoup(boardDescription,positionDest);

    if (indiceSrc == -1 || indiceDest == -1 || indiceSrc == indiceDest) {
        return 0;
    }

    return verifCoup(boardDescription,indiceSrc, indiceDest, color);
}
int verifCoup(BoardDescription *boardDescription,int indiceSrc, int indiceDest, int color) {

    uint64_t colorBitboard =
            boardDescription->boards[color][0] | boardDescription->boards[color][1] |
            boardDescription->boards[color][2] |
            boardDescription->boards[color][3] | boardDescription->boards[color][4] |
            boardDescription->boards[color][5];
    if ((colorBitboard & (uint64_t) pow(2,indiceDest)) != 0) {
        return 0;
    }

    int piece;
    int piece_temp;
    piece = findPiece(boardDescription,indiceSrc,color);
    switch (piece) {
        case -1 :
            return 0;
        case 0 :
            if(!pawnValidation(boardDescription,indiceSrc, indiceDest, color)){
                return 0;
            }
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest,color);
                movePiece(boardDescription,indiceDest,indiceSrc,piece,color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest,piece_temp,color);
                return 0;
            }
            int sign = 1;
            if (color == 1){
                sign = -1;
            }
            if(indiceDest == boardDescription->enPassant){
                killPiece(boardDescription,indiceDest+(8*-sign),color);
            }
            boardDescription->enPassant=64;
            if (indiceDest == (indiceSrc +(sign*16)))
                boardDescription->enPassant=indiceSrc+(8*sign);
            boardDescription->half_moves = 0;
            if ((indiceDest>=0 && indiceDest<=7) || (indiceDest>=56 && indiceDest<=63))
                return 2;
            break;
        case 1 :
            if (!knightValidation(boardDescription,indiceSrc,indiceDest))
                return 0;
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest, color);
                movePiece(boardDescription,indiceDest, indiceSrc, piece, color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest, piece_temp, color);
                return 0;
            }
            boardDescription->half_moves ++;
            if(piece_temp != 1)
                boardDescription->half_moves = 0;
            boardDescription->enPassant=64;
            break;
        case 2 :
            if (!bishopValidation(boardDescription,indiceSrc,indiceDest))
                return 0;
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest, color);
                movePiece(boardDescription,indiceDest, indiceSrc, piece, color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest, piece_temp, color);
                return 0;
            }
            boardDescription->half_moves ++;
            if(piece_temp != 1)
                boardDescription->half_moves = 0;
            boardDescription->enPassant=64;
            break;
        case 3 :
            if (!rookValidation(boardDescription,indiceSrc,indiceDest))
                return 0;
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest,color);
                movePiece(boardDescription,indiceDest,indiceSrc,piece,color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest,piece_temp,color);
                return 0;
            }
            if (indiceSrc == 0)
                boardDescription->castling = (boardDescription->castling & 0b1110);
            if (indiceSrc == 7)
                boardDescription->castling = (boardDescription->castling & 0b1101);
            if (indiceSrc == 56)
                boardDescription->castling = (boardDescription->castling & 0b1011);
            if (indiceSrc == 63)
                boardDescription->castling = (boardDescription->castling & 0b0111);
            boardDescription->enPassant=64;
            boardDescription->half_moves ++;
            if(piece_temp != 1)
                boardDescription->half_moves = 0;
            break;
        case 4 :
            if (!queenValidation(boardDescription,indiceSrc,indiceDest))
                return 0;
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest, color);
                movePiece(boardDescription,indiceDest, indiceSrc, piece, color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest, piece_temp, color);
                return 0;
            }
            boardDescription->half_moves ++;
            if(piece_temp != 1)
                boardDescription->half_moves = 0;
            boardDescription->enPassant=64;
            break;
        case 5 :
            if (!kingValidation(boardDescription,indiceSrc,indiceDest)&& !kingCastling(boardDescription,indiceSrc,indiceDest,color))
                return 0;
            piece_temp = killPiece(boardDescription,indiceDest,color);
            movePiece(boardDescription,indiceSrc,indiceDest,piece,color);
            if (isCheck(boardDescription,color)) {
                killPiece(boardDescription,indiceDest, color);
                movePiece(boardDescription,indiceDest, indiceSrc, piece, color);
                if (piece_temp != -1)
                    resurrectPiece(boardDescription,indiceDest, piece_temp, color);
                return 0;
            }
            if (color == 0 )
                boardDescription->castling = (boardDescription->castling & 0b1100);
            else
                boardDescription->castling = (boardDescription->castling & 0b0011);
            boardDescription->half_moves ++;
            if(piece_temp != 1)
                boardDescription->half_moves = 0;
            boardDescription->enPassant=64;
            break;
        default:
            return 0;

    }


    return 1;
}


//return  1  = knight
//return  2  = bishop
//return  3  = rook
//return  4  = queen
bool promotion(BoardDescription *boardDescription, char *positionDest, int color, int piece){

    int  indiceDest = validerCoup(boardDescription,positionDest);
    if (indiceDest == -1 ) {
        return false;
    }
    if (piece>0 && piece<5){
        boardDescription->boards[color][piece] = boardDescription->boards[color][piece] + (uint64_t) pow(2,indiceDest);
        boardDescription->boards[color][0] = boardDescription->boards[color][0] - (uint64_t) pow(2,indiceDest);
        return true; 
    }
    return false;
}


bool stalemate(BoardDescription *boardDescription,int color){

    BoardDescription *boardDescriptionCopy = malloc(sizeof(BoardDescription));
    for (int i = 0; i < 2; ++i){
        for (int j = 0; j < 6; ++j){
            boardDescriptionCopy->boards[i][j] = boardDescription->boards[i][j];
        }
    }
    uint64_t colorBitboard =
            boardDescription->boards[color][0] | boardDescription->boards[color][1] |
            boardDescription->boards[color][2] |
            boardDescription->boards[color][3] | boardDescription->boards[color][4] |
            boardDescription->boards[color][5];
    uint64_t pos = 1;
    for (int i=0; i<64 ; i++){
        //Find the piece
        if ((colorBitboard & pos) != 0){
            for (int j=0; j<64 ; j++){
                if(verifCoup(boardDescriptionCopy,i,j,color)!=0)
                    return 0;
            }
        }
        pos = pos <<1;
    }        
    return 1;
}


int isOver(BoardDescription *boardDescription,int color){
    if (stalemate(boardDescription,color)){
        if (isCheck(boardDescription,color)){
            return 1;
        }
        return 2;
    }
    if(boardDescription->half_moves>50)
        return 3;
    return 0;
}