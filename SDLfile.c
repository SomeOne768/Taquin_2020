#include <stdio.h>
#include <SDL.h>
#include <SDL_types.h>
#include <SDL_keysym.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1280
#define HEIGHT 720
#define DIM 2

void pause(){
	int arret = 0;
	SDL_Event event;
	while(!arret){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				arret = 1;
				break;
			
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case 'q':
						arret = 1;
						break;
						
					case SDLK_ESCAPE:
						arret = 1;
						break;
						
					default:
						arret = 0;
						break;
				}
				break;

			default:
				arret = 0;
				break;
		}
	}
}

void AffichageSDL(int **M, int dim, SDL_Rect* decoupe, SDL_Surface * Image, SDL_Surface *ecran);
void decoupeImage(SDL_Rect *decoupe, SDL_Surface *Image1, SDL_Surface *ecran, int dim);

int main(){
	if(SDL_Init(SDL_INIT_VIDEO) == -1){
		fprintf(stderr, "Erreur SDL :%s\n", SDL_GetError());
		return -1;
	}
	
	SDL_Surface *ecran = NULL;
	
	if( (ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_RESIZABLE)) == NULL){
		fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	SDL_Surface * rectangle = NULL;
	if( (rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 100, 150, 32, 255, 255, 255, 0)) == NULL){
		fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran ->format, 0, 0, 0));
	SDL_FillRect(rectangle, NULL, SDL_MapRGB(rectangle->format, 255, 255, 255));
	
	SDL_Rect position;
	position.x = 50;
	position.y = 50;
	
	SDL_BlitSurface(rectangle, NULL, ecran, &position);
	SDL_WM_SetCaption("Jeu de Taquin", NULL);
	SDL_Flip(ecran);
	pause();

	
	SDL_Surface *Image1 = NULL;
	if( (Image1 = SDL_LoadBMP("Image/SDL5.bmp")) == NULL){
		fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	
	SDL_FreeSurface(rectangle);
	SDL_Rect *place =(SDL_Rect *) malloc(sizeof(SDL_Rect) * (DIM * DIM));
	place[0].x = 00;
	place[0].y = 00;
	SDL_BlitSurface(Image1, NULL, ecran, &place[0]);
	SDL_Flip(ecran);
	pause();
	
		
	
	/*
	SDL_Rect *decoupe = (SDL_Rect *)malloc(sizeof(SDL_Rect) * (DIM * DIM));
	for(int i =0; i< (DIM * DIM); i++){
		decoupe[i].x = (i%DIM) * (WIDTH / DIM);
		decoupe[i].y = (i/DIM) * (HEIGHT / DIM);
		decoupe[i].w = WIDTH / DIM;
		decoupe[i].h = HEIGHT / DIM;
		SDL_BlitSurface(Image1,&decoupe[i],ecran,&decoupe[i]);
	}
	SDL_Flip(ecran);*/
	SDL_Rect *decoupe;
	decoupe = (SDL_Rect *)malloc(sizeof(SDL_Rect) * (DIM * DIM));
	decoupeImage(decoupe, Image1, ecran, DIM);	
	int **M = malloc(sizeof(int *) * 2);
	M[0] = malloc(sizeof(int) * 2);M[0][0] = 0; M[0][1] = 2;
	M[1] = malloc(sizeof(int) * 2);M[1][0] = 3; M[1][1] = 1;
	
	AffichageSDL(M, DIM, decoupe, Image1, ecran);

	pause();
		
	


	

	SDL_Quit();
	return EXIT_SUCCESS;
}

void decoupeImage(SDL_Rect *decoupe, SDL_Surface *Image1, SDL_Surface *ecran, int dim){

	//On attribut les portions d'image à découepr ainsi que leur emplacement
	for(int i =0; i< (dim * dim); i++){
		decoupe[i].x = (i%dim) * (WIDTH / dim);
		decoupe[i].y = (i/dim) * (HEIGHT / dim);
		decoupe[i].w = WIDTH / dim;
		decoupe[i].h = HEIGHT / dim;
		SDL_BlitSurface(Image1,&decoupe[i],ecran,&decoupe[i]);
	}
	SDL_Flip(ecran);
}

void AffichageSDL(int **M, int dim, SDL_Rect* decoupe, SDL_Surface * Image1, SDL_Surface *ecran){
	//On affiche les images dans le bon ordre en se servant des coefficients de la matrice 
	for(int i=0; i< (DIM * DIM); i++){
		if(M[(int)i/DIM][i%DIM] == 0){
			//Dernier image (case vide)
			SDL_BlitSurface(Image1, &decoupe[ (DIM * DIM) - 1], ecran,&decoupe[i]);
		}else{
			//Decalage de 1 car le 0 est le brd droit inférieur (case vide)
			SDL_BlitSurface(Image1, &decoupe[ M[(int)i/DIM][i%DIM] - 1], ecran,&decoupe[i]);
		}	
	}
	SDL_Flip(ecran);

}











