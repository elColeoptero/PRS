//
// Created by elcoleoptero on 12/04/2021.
//

#include "../chess_action/chess_action.h"
#include "../chess_display/chess_display.h"

/*******************************************************
Nom ......... : validerCoup
Role ........ :
Arg ......... : *boardDescription : 
                *position      : 
Return....... : uint64_t

********************************************************/

uint64_t validerCoup(BoardDescription *boardDescription, char *position)
{

    if (position[0] >= 'a' && position[0] <= 'h' && position[1] >= '1' && position[1] <= '8')
    {
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

/*******************************************************
Nom ......... : findPiece
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                color : 
Return....... : int8_t

********************************************************/
int8_t findPiece(BoardDescription *boardDescription, int indiceSrc, int color)
{
    uint64_t bitboardSrc = (uint64_t)pow(2, indiceSrc);
    uint64_t halfColorBitboard, colorBitboard;
    int halfIndex, piece;
    colorBitboard =
        boardDescription->boards[color][0] | boardDescription->boards[color][1] |
        boardDescription->boards[color][2] |
        boardDescription->boards[color][3] | boardDescription->boards[color][4] |
        boardDescription->boards[color][5];
    // Test if player want to move piece of his color
    if ((colorBitboard & bitboardSrc) == 0)
    {

        return -1;
    }
    // Find out which half the chess piece is in
    halfColorBitboard = boardDescription->boards[color][0] | boardDescription->boards[color][1] |
                        boardDescription->boards[color][2];
    if ((halfColorBitboard & bitboardSrc) != 0)
    {
        halfIndex = 0;
    }
    else
    {
        halfIndex = 1;
    }
    //Find the piece
    if ((boardDescription->boards[color][0 + halfIndex * 3] & bitboardSrc) != 0)
        piece = halfIndex * 3 + 0;
    else if ((boardDescription->boards[color][1 + halfIndex * 3] & bitboardSrc) != 0)
        piece = halfIndex * 3 + 1;
    else
        piece = halfIndex * 3 + 2;
    return piece;
}

/*******************************************************
Nom ......... : rookValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
Return....... : bool

********************************************************/

bool rookValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest)
{
    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];
    uint64_t rookAttacks = rook_attacks(indiceSrc, fullBitboard);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    if ((rookAttacks & destinationBitboard) == 0)
    {
        return false;
    }
    return true;
}

/*******************************************************
Nom ......... : bishopValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
Return....... : bool

********************************************************/

bool bishopValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest)
{
    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];
    uint64_t rookAttacks = calc_bishop_attacks(indiceSrc, fullBitboard);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    bb_to_str(fullBitboard);
    bb_to_str(rookAttacks);
    bb_to_str(destinationBitboard);
    bb_to_str(rookAttacks & destinationBitboard);
    if ((rookAttacks & destinationBitboard) == 0)
    {
        return false;
    }
    return true;
}

/*******************************************************
Nom ......... : queenValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
Return....... : bool

********************************************************/

bool queenValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest)
{
    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];
    uint64_t rookAttacks = queen_attacks(indiceSrc, fullBitboard);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    if ((rookAttacks & destinationBitboard) == 0)
    {
        return false;
    }
    return true;
}

/*******************************************************
Nom ......... : knightValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
Return....... : bool

********************************************************/

bool knightValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest)
{
    uint64_t rookAttacks = knight_attacks(indiceSrc);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    if ((rookAttacks & destinationBitboard) == 0)
    {
        return false;
    }
    return true;
}
//2
//6
//62
//58

/*******************************************************
Nom ......... : kingCastling
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
                color : 
Return....... : bool

********************************************************/

bool kingCastling(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int color)
{
    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];
    uint64_t colorAttack = colorAtacks(boardDescription, (color + 1) % 2);
    if (indiceSrc == 4)
    {
        // Grand rock blanc
        if (indiceDest == 2 && ((boardDescription->castling & 0b1) != 0))
        {
            if ((fullBitboard & (uint64_t)pow(2, 1)) == 0 && (fullBitboard & (uint64_t)pow(2, 2)) == 0 && (fullBitboard & (uint64_t)pow(2, 3)) == 0)
            {
                if ((colorAttack & (uint64_t)pow(2, 2)) == 0 && (colorAttack & (uint64_t)pow(2, 3)) == 0 && (colorAttack & (uint64_t)pow(2, 4)) == 0)
                {
                    movePiece(boardDescription, 0, 3, 3, color);
                    return true;
                }
            }
        }
        // Petit rock blanc
        if (indiceDest == 6 && ((boardDescription->castling & 0b10) != 0))
        {
            if ((fullBitboard & (uint64_t)pow(2, 5)) == 0 && (fullBitboard & (uint64_t)pow(2, 6)) == 0)
            {
                if ((colorAttack & (uint64_t)pow(2, 4)) == 0 && (colorAttack & (uint64_t)pow(2, 5)) == 0 && (colorAttack & (uint64_t)pow(2, 6)) == 0)
                {
                    movePiece(boardDescription, 7, 5, 3, color);
                    return true;
                }
            }
        }
    }
    if (indiceSrc == 60)
    {
        // Grand rock noir
        if (indiceDest == 58 && ((boardDescription->castling & 0b100) != 0))
        {
            if ((fullBitboard & (uint64_t)pow(2, 57)) == 0 && (fullBitboard & (uint64_t)pow(2, 58)) == 0 && (fullBitboard & (uint64_t)pow(2, 59)) == 0)
            {
                if ((colorAttack & (uint64_t)pow(2, 58)) == 0 && (colorAttack & (uint64_t)pow(2, 59)) == 0 && (colorAttack & (uint64_t)pow(2, 60)) == 0)
                {
                    movePiece(boardDescription, 56, 59, 3, color);
                    return true;
                }
            }
        }
        // Petit rock noir
        if (indiceDest == 62 && ((boardDescription->castling & 0b1000) != 0))
        {
            if ((fullBitboard & (uint64_t)pow(2, 61)) == 0 && (fullBitboard & (uint64_t)pow(2, 62)) == 0)
            {
                if ((colorAttack & (uint64_t)pow(2, 60)) == 0 && (colorAttack & (uint64_t)pow(2, 61)) == 0 && (colorAttack & (uint64_t)pow(2, 62)) == 0)
                {
                    movePiece(boardDescription, 63, 61, 3, color);
                    return true;
                }
            }
        }
    }
    return false;
}

/*******************************************************
Nom ......... : kingValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
Return....... : bool

********************************************************/

bool kingValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest)
{

    uint64_t rookAttacks = king_attacks(indiceSrc);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    if ((rookAttacks & destinationBitboard) == 0)
    {

        return false;
    }
    return true;
}
// TODO : pawn promotion

/*******************************************************
Nom ......... : pawnValidation
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc      : 
                indiceDest :
                color : 
Return....... : bool

********************************************************/

bool pawnValidation(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int color)
{

    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];

    uint64_t pawnAttacks = pawn_attacks(color, indiceSrc);
    uint64_t destinationBitboard = (uint64_t)pow(2, indiceDest);
    int sign, line;
    if (color == 0)
    {
        sign = 1;
        line = 7;
    }
    else
    {
        sign = -1;
        line = 47;
    }

    // TODO : pawn +1
    if ((indiceSrc + (8 * sign)) == indiceDest && (fullBitboard & destinationBitboard) == 0)
    {
        return true;
    }
    // TODO : pawn +2
    if ((indiceSrc + (16 * sign)) == indiceDest && (fullBitboard & destinationBitboard) == 0 && indiceSrc > line && indiceSrc < (line + 9))
    {
        return true;
    }
    // TODO : pawn attacks
    // TODO : EN PASSANT
    if ((pawnAttacks & destinationBitboard) != 0)
        if ((fullBitboard & destinationBitboard) != 0 || indiceDest == boardDescription->enPassant)
        {
            return true;
        }
    return false;
}

/*******************************************************
Nom ......... : colorAtacks
Role ........ :
Arg ......... : *boardDescription : 
                color : 
Return....... : uint64_t

********************************************************/

uint64_t colorAtacks(BoardDescription *boardDescription, int color)
{
    uint64_t attacks = 0;
    uint64_t pos = 1;
    uint64_t fullBitboard =
        boardDescription->boards[0][0] | boardDescription->boards[0][1] | boardDescription->boards[0][2] |
        boardDescription->boards[0][3] | boardDescription->boards[0][4] | boardDescription->boards[0][5] |
        boardDescription->boards[1][0] | boardDescription->boards[1][1] | boardDescription->boards[1][2] |
        boardDescription->boards[1][3] | boardDescription->boards[1][4] | boardDescription->boards[1][5];
    printf("JE RENTRE DANS colorAtacks\n");
    printf("%d\n", color);
    for (int i = 0; i < 64; i++)
    {
        //Find the piece
        if ((boardDescription->boards[color][0] & pos) != 0)
            attacks = (attacks | pawn_attacks(color, i));
        else if ((boardDescription->boards[color][1] & pos) != 0)
            attacks = (attacks | knight_attacks(i));
        else if ((boardDescription->boards[color][2] & pos) != 0)
            attacks = (attacks | calc_bishop_attacks(i, fullBitboard));
        else if ((boardDescription->boards[color][3] & pos) != 0)
            attacks = (attacks | calc_rook_attacks(i, fullBitboard));
        else if ((boardDescription->boards[color][4] & pos) != 0)
            attacks = (attacks | calc_bishop_attacks(i, fullBitboard) | calc_rook_attacks(i, fullBitboard));
        else if ((boardDescription->boards[color][5] & pos) != 0)
            attacks = (attacks | king_attacks(i));

        pos = pos << 1;
    }
    printf("\nJE RENTRE DANS colorAtacks\n");
    return attacks;
}

/*******************************************************
Nom ......... : isCheck
Role ........ :
Arg ......... : *boardDescription : 
                color : 
Return....... : bool

********************************************************/

bool isCheck(BoardDescription *boardDescription, int color)
{
    printf("JE RENTRE DANS ISCHECK\n");
    if ((colorAtacks(boardDescription, (color + 1) % 2) & boardDescription->boards[color][5]) != 0)
    {
        return true;
    }
    return false;
    printf("JE SORT DE ISCHECK\n");
}

/*******************************************************
Nom ......... : movePiece
Role ........ :
Arg ......... : *boardDescription : 
                indiceSrc :
                indiceDest :
                piece :
                color : 
Return....... : void

********************************************************/
void movePiece(BoardDescription *boardDescription, int indiceSrc, int indiceDest, int piece, int color)
{
    //printf("%d -- %d ",indiceSrc,indiceDest);
    boardDescription->boards[color][piece] = boardDescription->boards[color][piece] - (uint64_t)pow(2, indiceSrc);
    boardDescription->boards[color][piece] = boardDescription->boards[color][piece] + (uint64_t)pow(2, indiceDest);
}

/*******************************************************
Nom ......... : killPiece
Role ........ :
Arg ......... : *boardDescription : 
		        indiceDest : 
                color : 
Return....... : int

********************************************************/
int killPiece(BoardDescription *boardDescription, int indiceDest, int color)
{
    uint64_t pos = (uint64_t)pow(2, indiceDest);
    for (int i = 0; i < 6; ++i)
    {
        if ((boardDescription->boards[(color + 1) % 2][i] & pos) != 0)
        {
            boardDescription->boards[(color + 1) % 2][i] = boardDescription->boards[(color + 1) % 2][i] - (uint64_t)pow(2, indiceDest);
            return i;
        }
    }
    return -1;
}

/*******************************************************
Nom ......... : isCheck
Role ........ :
Arg ......... : *boardDescription : 
                color : 
Return....... : bool

********************************************************/
void resurrectPiece(BoardDescription *boardDescription, int indiceDest, int piece, int color)
{
    printf("ENTRE\n");
    printf("%d\n", indiceDest);
    printf("%d\n", piece);
    printf("%d\n", color);
    printf("%d\n", (color + 1) % 2);
    boardDescription->boards[(color + 1) % 2][piece] = boardDescription->boards[(color + 1) % 2][piece] + (uint64_t)pow(2, indiceDest);
    printf("SORTIE\n");
}
