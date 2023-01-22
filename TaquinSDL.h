#include <stdio.h>
#include <SDL.h>
#include <SDL_types.h>
#include <SDL_keysym.h>
#include <stdlib.h>
#include <time.h>
#ifndef TAQUINSDL
#define TAQUINSDL

void pause();
SDL_Surface** AttributionImage(int dim);
SDL_Rect* AttributionPosition(int dim);
void pauseMouvement(SDL_Surface *ecran, SDL_Surface **Image, SDL_Rect *position, Plateau *A, int dim, int i);
void adaptationMatrice(SDL_Surface **Image, int **M, int dimMatrice);


//ici pour dim>=5
void decoupeImage(SDL_Rect *decoupe, SDL_Surface *Image1, SDL_Surface *ecran, int dim, int WIDTH, int HEIGHT);
void AffichageSDL(int **M, int dim, SDL_Rect* decoupe, SDL_Surface * Image1, SDL_Surface *ecran);
void PauseSDL2Image(SDL_Surface *ecran, SDL_Surface *Image, SDL_Rect *decoupe, Plateau *A, int dim);

#endif
