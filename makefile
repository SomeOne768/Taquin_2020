OBJ = matrice.o main.o plateau.o TaquinSDL.o Presentation.o
FLAGS = -Wall -Wextra
Taquin:  matrice.o main.o plateau.o TaquinSDL.o Presentation.o
	gcc -o Taquin matrice.o main.o plateau.o Presentation.o TaquinSDL.o $(FLAGS) `sdl-config --cflags --libs`

matrice.o: matrice.c
	gcc -o matrice.o -c matrice.c $(FLAGS)

plateau.o: plateau.c matrice.h
	gcc -o plateau.o -c plateau.c  $(FLAGS)

TaquinSDL.o: TaquinSDL.c
	gcc -o TaquinSDL.o -c TaquinSDL.c $(FLAGS) `sdl-config --cflags --libs`

Presentation.o: Presentation.c
	gcc -o Presentation.o -c Presentation.c $(FLAGS) `sdl-config --cflags --libs`
	
main.o: main.c plateau.h matrice.h TaquinSDL.h Presentation.h
	gcc -o main.o -c main.c $(FLAGS) `sdl-config --cflags --libs`

clean:
	rm -rf $(OBJ)
