//
// Created by elcoleoptero on 30/03/2021.
//

#include "chess_game.h"


BoardDescription boardDescription;

void initBoard() {

    boardDescription.boards[0][0] = 0b0000000000000000000000000000000000000000000000001111111000000000;
    boardDescription.boards[0][1] = 0b0000000000000000000000000000000000000000000000000000000001000010;
    boardDescription.boards[0][2] = 0b0000000000000000000000000000000000000000000000000000000000100100;
    boardDescription.boards[0][3] = 0b0000000000000000000000000000000000000000000000000000000010000001;
    boardDescription.boards[0][4] = 0b0000000000000000000000000000000000000000000000000000000000001000;
    boardDescription.boards[0][5] = 0b0000000000000000000000000000000000000000000000000000000000010000;
    boardDescription.boards[1][0] = 0b0000000011111111000000000000000000000000000000000000000000000000;
    boardDescription.boards[1][1] = 0b0100001000000000000000000000000000000000000000000000000000000000;
    boardDescription.boards[1][2] = 0b0010010000000000000000000000000000000000000000000000000000000000;
    boardDescription.boards[1][3] = 0b1000000100000000000000000000000000000000000000000000000000000000;
    boardDescription.boards[1][4] = 0b0001000000000000000000000000000000000000000000000000000000000000;
    boardDescription.boards[1][5] = 0b0000100000000000000000000000000000000000000000000000000000000000;

    boardDescription.enPassant = 64; // no enPassant allowed
    boardDescription.half_moves = 0;
    boardDescription.rook = 0b00001111; // 4 rocks are possible

    init();

};

void displayGame(int player) {
    int tab[64];
    bitBoardToArray(boardDescription.boards, tab);
    displayBoard(tab);
}

bool jouerCoup(char *positionSrc, char *positionDest) {
    int  indiceSrc = validerCoup(positionSrc);
    int  indiceDest = validerCoup(positionDest);
    int playerToMove = 0;

    if (indiceSrc == -1 || indiceDest == -1 || indiceSrc == indiceDest) {
        return false;
    }

    uint64_t colorBitboard =
            boardDescription.boards[playerToMove][0] | boardDescription.boards[playerToMove][1] |
            boardDescription.boards[playerToMove][2] |
            boardDescription.boards[playerToMove][3] | boardDescription.boards[playerToMove][4] |
            boardDescription.boards[playerToMove][5];
    if ((colorBitboard & (uint64_t) pow(2,indiceDest)) != 0) {
        return false;
    }
    int piece;
    piece = findPiece(indiceSrc);


    switch (piece) {
        case -1 :
            return false;
        case 0 :
            printf("pawn not implemented");
            break;
        case 1 :
            printf("knight not implemented");
            break;
        case 2 :
            printf("bishop not implemented");
            break;
        case 3 :
            printf("rook implementation ");
            return rookValidation(indiceSrc,indiceDest);
            break;
        case 4 :
            printf("queen not implemented");
            break;
        case 5 :
            printf("king not implemented");
            break;
        default:
            return false;

    }

    //TODO : déplacement de la piéce

    if (isCheck(0)){
        //TODO déplament inverse
    }

    //TODO: on continue


    return true;
}

uint64_t validerCoup(char *position) {

    if (position[0] >= 'a' && position[0] <= 'h' && position[1] >= '1' && position[1] <= '8') {
        return position[0] - 97 + 8 * (position[1] - 49);
    }
    return -1;
}

//return  -1 = piece not found
//return  0  = pawn
//return  1  = knight
//return  2  = bishop
//return  3  = rook
//return  4  = queen
//return  5  = king
int8_t findPiece(int indiceSrc) {
    uint64_t bitboardSrc = pow(2,indiceSrc);
    uint64_t halfColorBitboard, colorBitboard;
    int halfIndex, piece, playerToMove = 0;
    colorBitboard =
            boardDescription.boards[playerToMove][0] | boardDescription.boards[playerToMove][1] |
            boardDescription.boards[playerToMove][2] |
            boardDescription.boards[playerToMove][3] | boardDescription.boards[playerToMove][4] |
            boardDescription.boards[playerToMove][5];
    // Test if player want to move piece of his color
    if ((colorBitboard & bitboardSrc) == 0) {
        return -1;
    }
    // Find out which half the chess piece is in
    halfColorBitboard = boardDescription.boards[playerToMove][0] | boardDescription.boards[playerToMove][1] |
                        boardDescription.boards[playerToMove][2];
    if ((halfColorBitboard & bitboardSrc) != 0) {
        halfIndex = 0;
    } else {
        halfIndex = 1;
    }
    //Find the piece
    if ((boardDescription.boards[playerToMove][0 + halfIndex * 3] & bitboardSrc) != 0)
        piece = halfIndex * 3 + 0;
    else if ((boardDescription.boards[playerToMove][1 + halfIndex * 3] & bitboardSrc) != 0)
        piece = halfIndex * 3 + 1;
    else
        piece = halfIndex * 3 + 2;
    return piece;
}

bool rookValidation(int indiceSrc, int indiceDest) {
    uint64_t fullBitboard =
            boardDescription.boards[0][0] | boardDescription.boards[0][1] | boardDescription.boards[0][2] |
            boardDescription.boards[0][3] | boardDescription.boards[0][4] | boardDescription.boards[0][5] |
            boardDescription.boards[1][0] | boardDescription.boards[1][1] | boardDescription.boards[1][2] |
            boardDescription.boards[1][3] | boardDescription.boards[1][4] | boardDescription.boards[1][5];
    uint64_t rookAttacks = rook_attacks(indiceSrc,fullBitboard);
    uint64_t destinationBitboard = pow(2,indiceDest);
    if ((rookAttacks & destinationBitboard) ==0){
        return false;
    }

    return true;
}

uint64_t colorAtacks(int color){

    uint64_t attacks=0;
    uint64_t pos =1;
    uint64_t fullBitboard =
            boardDescription.boards[0][0] | boardDescription.boards[0][1] | boardDescription.boards[0][2] |
            boardDescription.boards[0][3] | boardDescription.boards[0][4] | boardDescription.boards[0][5] |
            boardDescription.boards[1][0] | boardDescription.boards[1][1] | boardDescription.boards[1][2] |
            boardDescription.boards[1][3] | boardDescription.boards[1][4] | boardDescription.boards[1][5];
    for (int i=0; i<64 ; i++){


        //Find the piece
        if ((boardDescription.boards[color][0] & pos) != 0)
            attacks = (attacks | pawn_attacks(color,i));
        else if ((boardDescription.boards[color][1] & pos) != 0)
            attacks = (attacks | knight_attacks(i));
        else if ((boardDescription.boards[color][2] & pos) != 0)
            attacks = (attacks | bishop_attacks(i,fullBitboard));
        else if ((boardDescription.boards[color][3] & pos) != 0)
            attacks = (attacks | rook_attacks(i,fullBitboard));
        else if ((boardDescription.boards[color][4] & pos) != 0)
            attacks = (attacks | queen_attacks(i,fullBitboard));
        else if ((boardDescription.boards[color][5] & pos) != 0)
            attacks = (attacks | king_attacks(i));

        pos = pos <<1;
    }
}
bool isCheck(int color){
    if ((colorAtacks((color+1)%2) & boardDescription.boards[color][5]) != 0 ){
        return true;
    }
    return false;
}

void pieceMouving(int indiceSrc, int indiceDest, int piece, int color){

    boardDescription.boards[color][piece] -= pow(2,indiceSrc);
    boardDescription.boards[color][piece] += pow(2,indiceDest);
}

