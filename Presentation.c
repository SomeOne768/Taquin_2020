#include <stdio.h>
#include <SDL.h>
int menuChargement(){
	printf("Bonjour et bienvenu sur notre jeu de Taquin.\n");
	printf("Souhaitez-vous :\n");
	printf("1 - Commencer une nouvelle partie.\n");
	printf("2 - Récupérer une partie sauvegardée ?\n");
	int b=0;
	char c;
	do{
		if(b){
			printf("Saisie incorrect, veuillez taper 1 ou 2.\n");
		}
		scanf(" %c", &c);
		b = 1;
	}while( (c != '1') && (c != '2') );
	if(c == '1'){
		//Nouvelle partie
		return 1;
	}
	return 0;
}
	
int menuSDL(){
	printf("\n\nSouhaitez-vous utiliser la SDL (graphique) ou préférez-vous un affichage textuel (terminal).\n");
	printf("1 - SDL. (Valable pour des dimensions < 5)\n");
	printf("2 - Affichage textuel.\n");
	char c;
	int b = 0;
	do{
		if(b){
			printf("Saisie incorrect, veuillez taper 1 ou 2.\n");
		}
		scanf(" %c", &c);
	}while( (c != '1') && (c != '2') );
	if(c == '1'){
		//SDL
		return 1;
	}
	return 0;
}

void SDLPresentation(SDL_Surface *ecran, float WIDTH, float HEIGHT){
	//Recuperation de l'image dans son dossier
	SDL_Surface *Presentation = NULL;
	if( (Presentation = SDL_LoadBMP("Image/Presentation.bmp")) == NULL){
		fprintf(stderr, "Erreur Image %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_Rect position;
	position.x = (WIDTH - 750) / 2;
	position.y = (HEIGHT - 750) / 2;
	
	//1er affichage
	SDL_BlitSurface(Presentation, NULL, ecran, &position);
	SDL_Flip(ecran);
	free(Presentation);
	int next = 0;
	SDL_Event event;
	while(!next){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				next = 1;
				break;
			
			case SDL_KEYDOWN:
			
				switch(event.key.keysym.sym){
					//les 3 cas produisent la même chose à savoir :
					// Appuyer sur espace, q ou échap quitte la fenetre pour passer à la suivante
					case SDLK_SPACE:
						Presentation = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32,0,0,0,0) ;
						SDL_FillRect(Presentation, NULL, SDL_MapRGB(Presentation->format, 0, 0, 0));
						SDL_BlitSurface(Presentation, NULL, ecran, &position);
						SDL_Flip(ecran);
						free(Presentation);	
						next = 1;
						break;
					
					case SDLK_ESCAPE:
						Presentation = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32,0,0,0,0) ;
						SDL_FillRect(Presentation, NULL, SDL_MapRGB(Presentation->format, 0, 0, 0));
						SDL_BlitSurface(Presentation, NULL, ecran, &position);
						SDL_Flip(ecran);
						free(Presentation);	
						next = 1;
						break;
					
					case SDLK_q:
						Presentation = SDL_CreateRGBSurface(SDL_HWSURFACE, WIDTH, HEIGHT, 32,0,0,0,0) ;
						SDL_FillRect(Presentation, NULL, SDL_MapRGB(Presentation->format, 0, 0, 0));
						SDL_BlitSurface(Presentation, NULL, ecran, &position);
						SDL_Flip(ecran);
						free(Presentation);	
						next = 1;
						break;
						
					default:
						break;
				}
				break;
			
			default:
				break;
		}
	}
}
		
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
