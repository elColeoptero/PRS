/*******************************************************
Nom ......... : chess_display.c
Role ........ : Fichier contenant le nécessaire pour 
                l'affichage du jeu
Auteur ...... : POLAN
********************************************************/

#include "chess_display.h"

void displayBoard(int board_array[], int couleurJoueur, char nameJoueur[2][20])
{
    char *caracteres[] = {"\u265F ", "\u265E ", "\u2657 ", "\u265C ", "\u265B ", "\u265A ", "\u265F ", "\u265E ", "\u2657 ", "\u265C ", "\u265B ", "\u265A ", "  "};
    char *couleur[] = {"\033[0;37m", "\033[0;30m"};
    char *couleurs_fond[] = {"\033[43m", "\033[45m"};

    if (couleurJoueur == 0)
        printf(" A B C D E F G H\n");
    else
        printf(" H G F E D C B A\n");
    for (int i = 7; i > -1; i--)
    {
        if (couleurJoueur == 0)
            printf("\033[0;37m%d", i + 1);
        else
            printf("\033[0;37m%d", 8 - i);
        int case_couleur = i % 2;
        for (int j = 0; j < 8; j++)
        {
            case_couleur = (case_couleur + 1) % 2;
            int indice = i * 8 + j;
            if (couleurJoueur == 1)
                indice = 63 - indice;
            int numero_piece = board_array[indice];
            char *caractere_piece = caracteres[numero_piece];
            int indice_couleur = (numero_piece / 6) % 2;
            char *couleur_piece = couleur[indice_couleur];
            char *couleur_fond = couleurs_fond[case_couleur];
            printf("%s%s%s", couleur_piece, couleur_fond, caractere_piece);
        }
        printf("\033[0m");
        if (i == 7)
        {
            if (couleurJoueur == 1)
                printf("   %s\n", nameJoueur[0]);
            else
                printf("   %s\n", nameJoueur[1]);
        }
        else if (i == 0)
        {
            if (couleurJoueur == 1)
                printf("   %s\n", nameJoueur[1]);
            else
                printf("   %s\n", nameJoueur[0]);
        }
        else
            printf("\n");
    }
}

void bitBoardToArray(uint64_t boards[2][6], int tab[])
{
    uint64_t board, white, moitie, pos = 1;
    int piece, half, color;
    for (int i = 0; i < 64; i++)
    {
        board = boards[0][0] | boards[0][1] | boards[0][2] | boards[0][3] | boards[0][4] | boards[0][5] | boards[1][0] | boards[1][1] | boards[1][2] | boards[1][3] | boards[1][4] | boards[1][5];

        if ((board & pos) == 0)
            tab[i] = 12;
        else
        {
            // test 1 --> couleur ?
            white = boards[0][0] | boards[0][1] | boards[0][2] | boards[0][3] | boards[0][4] | boards[0][5];

            if ((white & pos) != 0)
                color = 0;
            else
                color = 1;
            // test 2 --> pion,cavalier,fou ?
            moitie = boards[color][0] | boards[color][1] | boards[color][2];

            if ((moitie & pos) != 0)
                half = 0;
            else
                half = 1;
            // test 3 --> (0-3) , (1-4) , (2-5)
            if ((boards[color][0 + half * 3] & pos) != 0)
                piece = color * 6 + half * 3 + 0;
            else if ((boards[color][1 + half * 3] & pos) != 0)
                piece = color * 6 + half * 3 + 1;
            else
                piece = color * 6 + half * 3 + 2;
            tab[i] = piece;
        }

        pos = pos << 1;
    }
}

void bb_to_str(uint64_t b1)
{
    for (int r = 7; r >= 0; --r)
    {
        printf("   +---+---+---+---+---+---+---+---+\n");
        printf(" %d |", r + 1);
        for (int f = 0; f <= 7; ++f)
        {
            if ((b1 & (1ULL << ((r << 3) | f))))
                printf(" X |");
            else
                printf("   |");
        }

        printf("\n");
    }
    printf("   +---+---+---+---+---+---+---+---+\n");
    printf("     a   b   c   d   e   f   g   h  \n");
}