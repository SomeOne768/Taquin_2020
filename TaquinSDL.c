#include <stdio.h>
#include <SDL.h>
#include <SDL_types.h>
#include <SDL_keysym.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "plateau.h"


void pause(){
	int arret = 0;
	SDL_Event event;
	while(!arret){
		//On quitte la fenetre seuleemnt si l'utilisateur appuie sur une touche ou la ferme lui même
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				arret = 1;
				break;
			
			case SDL_KEYDOWN:
				arret = 1;
				break;
			
			default:
				arret = 0;
				break;
		}
	}
}



SDL_Surface** AttributionImage(int dim){
	//On cherche la sauvegarde dans le dossier approprié
	char racine[] = "Image/";

	//Accès à la banque d'image
	char s[20] = "00.bmp";
	SDL_Surface **Timage = NULL;
	
	//On charge une image par case donc dim² image au total
	Timage = (SDL_Surface **)malloc(sizeof(SDL_Surface*) * (dim * dim));
	int numImageMax = (dim * dim) - 1; //Car 0 compte
	int i;
	for(i=0; i<=numImageMax; i++){
		int c = i/10;
		sprintf(s,"%s%d%d.bmp", racine, c, i%10);
		//Récupértion de l'image XX.bmp
		if( (Timage[i] = SDL_LoadBMP(s)) == NULL ){
			fprintf(stderr, "Erreur Image %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
		}
	}
	return Timage;
}

SDL_Rect* AttributionPosition(int dim){//Création d'un tableau de position pour les images
	SDL_Rect *position = NULL;
	//dim² image donc dim² position
	position = (SDL_Rect *)malloc(sizeof(SDL_Rect) * (dim * dim));
	int i;
	for(i=0; i<(dim * dim); i++){
		int decalbas = 225 * (int)(i / dim);
		int decaldroite = 225 * (i%dim);
		position[i].y = 10 + decalbas;
		position[i].x = 10 + decaldroite;
	}
	return position;
}
	
void EchangeImage(SDL_Surface **Image, int i, int j){
	//On echange fictivement les images en échangeant leurs adresses
	SDL_Surface *temp = NULL;
	temp = Image[i];
	Image[i] = Image[j];
	Image[j] = temp;
}


void pauseMouvement(SDL_Surface *ecran, SDL_Surface **Image, SDL_Rect *position, Plateau *A, int dim, int i){
	int arret = 0;
	int actualiser = 0;
	int **Mid = MatriceId(dim, dim);
	SDL_Event event;
	while(!arret){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				arret = 1;
				break;
			
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					//Mouvement avec les fleches ou g,d,h,b et quitter la partie
					case SDLK_LEFT:
						if(i%dim != 0){
							EchangeImage(Image, i, i-1);
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
							i = i - 1;
							actualiser = 1;
						}
						break;
					
					case SDLK_RIGHT:
						if(i%dim !=(dim -1)){
							EchangeImage(Image, i, i+1);
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
							i = i + 1;
							actualiser = 1;
						}
						break;
					
					
					case SDLK_DOWN:
						if( i < (dim * (dim - 1)) ){
							EchangeImage(Image, i, i+dim);
							echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
							i = i + dim;
							actualiser = 1;
						}
						break;
					
					case SDLK_UP:
						if(i>=dim){
							EchangeImage(Image, i, i-dim);
							echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
							i = i - dim;
							actualiser = 1;
						}
						break;
					
					case SDLK_q:
						arret = 1;
						break;
					
					case SDLK_ESCAPE:
						arret = 1;
						break;
					
					/*--------------------------------------------------*/
					//Pour respecter la consigne
					case SDLK_g:
						if(i%dim != 0){
							EchangeImage(Image, i, i-1);
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
							i = i - 1;
							actualiser = 1;
						}
						break;
					
					case SDLK_d:
						if(i%dim !=(dim -1)){
							EchangeImage(Image, i, i+1);
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
							i = i + 1;
							actualiser = 1;
						}
						break;
					
					
					case SDLK_b:
						if( i < (dim * (dim - 1)) ){
							EchangeImage(Image, i, i+dim);
							echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
							i = i + dim;
							actualiser = 1;
						}
						break;
					
					case SDLK_h:
						if(i>=dim){
							EchangeImage(Image, i, i-dim);
							echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
							i = i - dim;
							actualiser = 1;
						}
						break;
					/*--------------------------------------------------*/
					
					default:
						printf("Veuillez utiliser seulement les fleches directionnelles ou q pour quitter.\n");
						break;
				}					
				if(actualiser){
					//S'il y a eu un mouvment on l'affiche à l'écran
					int i; 
					for(i=0; i<(dim * dim); i++){
						SDL_BlitSurface(Image[i], NULL, ecran, &position[i]);
					}
					SDL_Flip(ecran);
					actualiser = 0;
				}
				break;
			
			default:
				arret = 0;
				break;
		}
		if(!estDifferente(A->M, Mid, dim)){
			arret = 1;
		}
	}//Fin du while
	Liberation(Mid, dim);
}

void adaptationMatrice(SDL_Surface **Image, int **M, int dimMatrice){
	int i,j;
	//On crée une copie
	SDL_Surface **temp = NULL;
	temp = (SDL_Surface **)malloc(sizeof(SDL_Surface *) * dimMatrice * dimMatrice);
	for(i=0; i<(dimMatrice * dimMatrice); i++){
		temp[i] = (SDL_Surface *)malloc(sizeof(SDL_Surface));
		temp[i] = Image[i];
	}
	
	//On attribue à la valeur i l'image du chiffre i
	for(i=0; i<dimMatrice; i++){
		for(j=0; j<dimMatrice; j++){
			Image[(dimMatrice * i) + j] = temp[M[i][j]];
		}
	}
	/*for(i=0; i<(dimMatrice * dimMatrice); i++){
		free(temp[i]);
	}*/ //Crée des problèmes (core dumped) alors que les indices sont ok
	free(temp);
}
	
	

void decoupeImage(SDL_Rect *decoupe, SDL_Surface *Image1, SDL_Surface *ecran, int dim, int WIDTH, int HEIGHT){

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

void AffichageSDL(int **M, int DIM, SDL_Rect* decoupe, SDL_Surface * Image1, SDL_Surface *ecran){
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

/*****************Fonction identique à l'autre pause mais ici pour l'image au lieu de chiffres******/

void PauseSDL2Image(SDL_Surface *ecran, SDL_Surface *Image, SDL_Rect *decoupe, Plateau *A, int DIMTAQUIN){

	int arret = 0;
	int i = (A->zero[0] * DIMTAQUIN) + A->zero[1];
	int actualiser = 0;
	int **Mid = MatriceId(DIMTAQUIN, DIMTAQUIN);
	//Représente l'emplacement du 0 (Ex M[1][2] = 0 <=> le 0 est en dans la 6eme case donc case n°5 )
	SDL_Event event;
	while(!arret){
		SDL_WaitEvent(&event);
		//On quitte la fenetre seulement si l'utilisateur appuie sur une touche ou la ferme lui même
		switch(event.type){
			case SDL_QUIT:
				arret = 1;
				break;
			
			
			case SDL_KEYDOWN:
				 switch(event.key.keysym.sym){
					//Mouvement avec les fleches et quitter la partie
					case SDLK_LEFT:
						if(i%DIMTAQUIN != 0){
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
							i = i - 1;
							actualiser = 1;
						}
						break;
					
					case SDLK_RIGHT:
						if(i%DIMTAQUIN !=(DIMTAQUIN -1)){
							echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
							i = i + 1;
							actualiser = 1;
						}
						break;
					
					
					case SDLK_DOWN:
						if( i < (DIMTAQUIN * (DIMTAQUIN - 1)) ){
							echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
							i = i + DIMTAQUIN;
							actualiser = 1;
						}
						break;
					
					case SDLK_UP:
						if(i>=DIMTAQUIN){
							echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
							i = i - DIMTAQUIN;
							actualiser = 1;
						}
						break;
					
					case SDLK_q:
						arret = 1;
						break;
					
					case SDLK_ESCAPE:
						arret = 1;
						break;
					
					default:
						break;
				}//fin du 2eme switch
				break;//Fin KEYDOWN
			
			default:
				break;
		}//fin du premier switch
		
		if(actualiser){
			//S'il y a eu un mouvement on l'affiche à l'écran
			AffichageSDL(A->M, DIMTAQUIN, decoupe, Image, ecran);
			SDL_Flip(ecran);
			actualiser = 0;
		}
		if(!estDifferente(A->M, Mid, DIMTAQUIN)){
			arret = 1;
		}
	}//fin du while
	Liberation(Mid, DIMTAQUIN);
}
