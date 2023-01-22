
Taquin:  matrice.o main.o plateau.o TaquinSDL.o Presentation.o
	gcc -o Taquin matrice.o main.o plateau.o Presentation.o TaquinSDL.o `sdl-config --cflags --libs`

matrice.o: matrice.c
	gcc -o matrice.o -c matrice.c -W -Wall

plateau.o: plateau.c matrice.h
	gcc -o plateau.o -c plateau.c  -W -Wall

TaquinSDL.o: TaquinSDL.c
	gcc -o TaquinSDL.o -c TaquinSDL.c `sdl-config --cflags --libs`

Presentation.o: Presentation.c
	gcc -o Presentation.o -c Presentation.c `sdl-config --cflags --libs`
	
main.o: main.c plateau.h matrice.h TaquinSDL.h Presentation.h
	gcc -o main.o -c main.c -W -Wall `sdl-config --cflags --libs`
