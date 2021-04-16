all: libchess libread
	gcc src/main.c -o exe -lpthread  -lchess -lread -Lbuild -lm
	chmod +s exe
libchess: chess_bitboard chess_display chess_game chess_action
	ar rcs build/libchess.a build/chess_bitboard.o build/chess_display.o build/chess_game.o build/chess_action.o
chess_bitboard:
	gcc src/lib/chess_bitboard/chess_bitboard.c -c -o build/chess_bitboard.o 
chess_display:
	gcc src/lib/chess_display/chess_display.c -c -o build/chess_display.o 
chess_game:
	gcc src/lib/chess_game/chess_game.c -c -o build/chess_game.o 
chess_action:
	gcc src/lib/chess_action/chess_action.c -c -o build/chess_action.o 
libread: read_lib
	ar rcs build/libread.a build/read_lib.o
read_lib:
	gcc src/lib/read_lib/read_lib.c -c -o build/read_lib.o
