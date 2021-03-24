#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
void displayBoard(int board_array[]);
void bitBoardToArray(uint64_t boards[2][6],int tab[]);
void bb_to_str(uint64_t b1);
int main(int argc, char const *argv[])
{
int tab[64];
uint64_t boards[2][6];
boards[0][0] = 0b0000000000000000000000000000000000000000000000001111111100000000;
boards[0][1] = 0b0000000000000000000000000000000000000000000000000000000001000010;
boards[0][2] = 0b0000000000000000000000000000000000000000000000000000000000100100;
boards[0][3] = 0b0000000000000000000000000000000000000000000000000000000010000001;
boards[0][4] = 0b0000000000000000000000000000000000000000000000000000000000001000;
boards[0][5] = 0b0000000000000000000000000000000000000000000000000000000000010000;

boards[1][0] = 0b0000000011111111000000000000000000000000000000000000000000000000;
boards[1][1] = 0b0100001000000000000000000000000000000000000000000000000000000000;
boards[1][2] = 0b0010010000000000000000000000000000000000000000000000000000000000;
boards[1][3] = 0b1000000100000000000000000000000000000000000000000000000000000000;
boards[1][4] = 0b0001000000000000000000000000000000000000000000000000000000000000;
boards[1][5] = 0b0000100000000000000000000000000000000000000000000000000000000000;

bitBoardToArray(boards,tab);
//int board_array[] = {3,1,2,4,5,2,1,3,0,0,0,0,0,0,0,0,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,6,6,6,6,6,6,6,6,9,7,8,10,11,8,7,9};
displayBoard(tab);
}

void displayBoard(int board_array[])
{
//int board_array[] = {3,1,2,4,5,2,1,3,0,0,0,0,0,0,0,0,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,6,6,6,6,6,6,6,6,9,7,8,10,11,8,7,9};
char* caracteres[] = {"\u265F ","\u265E ","\u2657 ","\u265C ","\u265B ","\u265A ","\u265F ","\u265E ","\u2657 ","\u265C ","\u265B ","\u265A ","  "};
char* couleur[]={"\033[0;37m","\033[0;30m"};
char* couleurs_fond[]={"\033[43m","\033[45m"};

printf(" A B C D E F G H\n");
for(int i=7; i>-1;i--){
printf("\033[0;37m%d",i+1);
int case_couleur = i%2;
for(int j=0; j<8; j++){
	case_couleur= (case_couleur+1)%2;
	int indice = i*8+j;
	int numero_piece = board_array[indice];
	char* caractere_piece = caracteres[numero_piece];
	int indice_couleur=(numero_piece/6)%2;
	char* couleur_piece = couleur[indice_couleur];
	char * couleur_fond=couleurs_fond[case_couleur];
	printf("%s%s%s",couleur_piece,couleur_fond, caractere_piece);

}
printf("\033[0m\n");
}
printf("%s ",couleur[0]);
}

void bitBoardToArray(uint64_t boards[2][6],int tab[])
{
uint64_t board,white,moitie,pos=1;
int piece,half,color; 
for (int i=0; i<64 ; i++){
	board = boards[0][0] | boards[0][1] | boards[0][2] | boards[0][3] | boards[0][4] | boards[0][5] | boards[1][0] | boards[1][1] | boards[1][2] | boards[1][3] | boards[1][4]|boards[1][5];

if ((board & pos) == 0)
tab[i]=12;
else {
	// test 1 --> couleur ? 
	white= boards[0][0] | boards[0][1] | boards[0][2] | boards[0][3] | boards[0][4] | boards[0][5];
	
	if ((white & pos) !=0)
		color = 0;
	else 
		color = 1;
	// test 2 --> pion,cavalier,fou ?
	moitie = boards[color][0] | boards[color][1] | boards[color][2];


	if ((moitie & pos) != 0)
		half = 0;
	else 
		half = 1 ;
	// test 3 --> (0-3) , (1-4) , (2-5)
	if ((boards[color][0+half*3] & pos )!= 0)
		piece = color*6 + half*3 + 0;
	else if ((boards[color][1+half*3] & pos )!= 0)
 		piece = color*6 + half*3 + 1;
	else
		piece = color*6 + half*3 + 2;
	tab[i]=piece;
}

	pos = pos <<1;
}

}


void bb_to_str(uint64_t b1)
{
  for (int r = 7; r >= 0; --r) {
    printf("   +---+---+---+---+---+---+---+---+\n");
    printf(" %d |",r + 1);
    for (int f = 0; f <= 7; ++f) {
      if ((b1 & (1ULL << ((r << 3) | f))))
        printf(" X |" );
      else 
        printf("   |");
    }
    
    printf("\n");
  }
  printf("   +---+---+---+---+---+---+---+---+\n");
  printf("     a   b   c   d   e   f   g   h  \n");

}



