#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrice.h"
#include "plateau.h"
#include "TaquinSDL.h"
#include "Presentation.h"
#include <SDL.h>
#include <SDL_types.h>
#include <SDL_keysym.h>
#include <time.h>


//#define DIMTAQUIN 4
#define WIDTH (4 * 225) + 20
#define HEIGHT WIDTH


int main (){
	int ChargementNewPartie = menuChargement();
	int *dim = (int *)malloc(sizeof(int));
	int estSauvegarde = 0;
	char *sauv = (char *)malloc(sizeof(char) * 21);
	
	// Chargement/Initialisation d'un plateau de jeu
	Plateau *A;
	if(!ChargementNewPartie){
		//On charge une partie
		printf("Donnez le nom du fichier sauvegardé.\n");		
		scanf(" %s", sauv);
		A = Recuperation(sauv, dim);
		//test de récupération
		if(A == NULL){
			printf("Cette partie n'existe pas\n");
			exit(EXIT_FAILURE);
		}	
		estSauvegarde = 1;
	}else{
		//on crée une nouvelle partie
		printf("Quelle dimension souhaitez-vous ?(>1)\n");
		scanf(" %d", dim);
		
		//Gestion des mauvaises saisies
		if(*dim <= 1){
			printf("Dimensions incorrect\n");
			exit(EXIT_FAILURE);
		}
		
		printf("\n\nCréation du plateau de jeu ..\n");
		A = PlateauId(*dim, *dim);
		PlateauRandom(A, *dim);
		printf("Voici le plateau de jeu :\n");
	}
	AfficheMatrice(A->M, *dim, *dim);
	
	//Jeu
	
	//Etape1: Choix SDL ou affichage Textuel
	int DIMTAQUIN = *dim;
	int sdl = menuSDL();// sdl = 1 et textuel = 0
	char reponse;
	int **Mid = MatriceId(DIMTAQUIN,DIMTAQUIN);//Permet de déterminer lorsque c'est résolu
	
	if(sdl){
		int chiffre;
		printf("Souhaitez-vous un affichage avec:\n");
		printf("1 - Des chiffres (dimension <=4 uniquement).\n");
		printf("2 - Une image ? Avant le mélange l'image vous sera présentée\n");
		do{
			scanf("%d", &chiffre);
		}while(chiffre != 1 && chiffre != 2);
		//Initialisation de la SDL
		if(SDL_Init(SDL_INIT_VIDEO) == -1){
			fprintf(stderr, "Erreur SDL :%s\n", SDL_GetError());
			return -1;
		}	
		
		/********************************************CAS DIM < 5 POUR CHIFFRE **********************/
		//Pas de différence algorithmique, pas utile à regardé si vous regardez le suivant
		
		
		if(DIMTAQUIN<5 && (chiffre == 1)){
		 	//ecran
			SDL_Surface *ecran = NULL;
			if( (ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_RESIZABLE)) == NULL){
				fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
			}
			
			SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
			SDL_WM_SetCaption("Jeu de Taquin",NULL) ;
			
			//Affichage
			SDLPresentation(ecran, WIDTH, HEIGHT);//Instruction utilisateur (touches à utiliser)
		 
			SDL_Surface **Image = NULL;
			Image = AttributionImage(DIMTAQUIN);//Tableau d'image
		
			SDL_Rect *position = NULL;
			position = AttributionPosition(DIMTAQUIN);//Position des images
			
			adaptationMatrice(Image, A->M, DIMTAQUIN);//Placement des images
			
			
			printf("\n\n\nVous avez choisi l'affichage SDL.\nVeuillez utilisez les fleches directionnelles.\n");
			
			
			//1ere affichage du plateau
			int i; 
			for(i=0; i<(DIMTAQUIN * DIMTAQUIN); i++){
				SDL_BlitSurface(Image[i], NULL, ecran, &position[i]);
			}
			SDL_Flip(ecran);
			
			
			//Récupération mouvement et actualisation affichage
			pauseMouvement(ecran, Image, position, A, DIMTAQUIN, (A->zero[0] * DIMTAQUIN) + A->zero[1]);
			
			//On libere la mémoire
			for(i=0; i<(DIMTAQUIN * DIMTAQUIN); i++){
				SDL_FreeSurface(Image[i]);
			}
			free(Image);
			
			//Test de victoire
			if(!estDifferente(A->M, Mid, DIMTAQUIN)){
				printf("Félicitation vous avez réussi !\n");
				
				//Image de félécitation
				SDL_Surface *Congratulation = NULL;
				if( (Congratulation = SDL_LoadBMP("Image/Congratulation2.bmp")) == NULL){
					fprintf(stderr, "Erreur Image %s\n", SDL_GetError());
					exit(EXIT_FAILURE);
				}
				position[0].x = (WIDTH - 617)/2;
				position[0].y = (HEIGHT - 717) /2;
				SDL_BlitSurface(Congratulation, NULL, ecran, &position[0]);
				SDL_Flip(ecran);
				SDL_Delay(5000);
				SDL_FreeSurface(Congratulation);
				free(position);
				position = NULL;			
			}
			
		
		/**********************************************************************************/
		
		}else{
		
		/*************************** DIM >=5 AFFICHAGE IMAGE ******************************/
		// Pas de grande différence niveau algorithmique pas utile à regarder si vous avez regardé l'autre
		
		
			SDL_Surface *ecran = NULL;
			if( (ecran = SDL_SetVideoMode(1280, 720, 32, SDL_HWSURFACE | SDL_RESIZABLE)) == NULL){
				fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
			}
			
			SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
			SDL_WM_SetCaption("Jeu de Taquin",NULL) ;
			SDLPresentation(ecran, 1280, 720);//Instruction utilisateur (touches à utiliser)
		
		
			SDL_Surface *Image = NULL;
			if( (Image = SDL_LoadBMP("Image/SDL5.bmp")) == NULL){
				fprintf(stderr, "Erreur VideoMode %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
			}
			
			SDL_Rect *decoupe = NULL;
			decoupe = (SDL_Rect *)malloc(sizeof(SDL_Rect) * ((DIMTAQUIN) * (DIMTAQUIN)));
			decoupeImage(decoupe, Image, ecran, DIMTAQUIN, 1280, 720);
			PauseSDL2Image(ecran, Image, decoupe, A, DIMTAQUIN);
			
		}//Fin du else (dim>=5)
		
		/*********************************************************************************/
		
		SDL_Quit();
		//Gestion de la sauvegarde
		printf("Sauvegarder votre partie ? o/n\n");
		scanf(" %c", &reponse);
		if(reponse == 'o'){
			if(estSauvegarde){
				SauvJeu(sauv, A, DIMTAQUIN);
			}else{
				printf("Donnez un nom au fichier (max 20. caractères et seule les lettres/chiffres sont autorisés).\n");
				scanf(" %s", sauv);
				SauvJeu(sauv, A, DIMTAQUIN);
				
			}
		}
	

	
	}else{//Else de sdl => textuel
		printf("\n\n\nVous avez choisi l'affichage textuel\n");
		int continuer = 1;
		while( estDifferente(A->M, Mid, DIMTAQUIN) && continuer ){
			//Tant que l'utilisateur souhaite jouer et que la partie n'est pas terminée
			printf("\nSaisissez g,d,h,b pour deplacer une pièce sur la case vide (r pour arrêter).\n\n");
			AfficheMatrice(A->M, DIMTAQUIN, DIMTAQUIN);
			scanf(" %c", &reponse);
			if(reponse != 'r'){	
				//mouvement
				Jeu(A, DIMTAQUIN, reponse);					
			}else{
				continuer = 0;
			}
		}
		if(continuer){
			//si continuer == 1 alors la partie est terminée
			AfficheMatrice(A->M, DIMTAQUIN, DIMTAQUIN);
			printf("\n\nFélicitation vous avez gagné !\n");
		}
		printf("Sauvegarder votre partie ? o/n\n");
		scanf(" %c", &reponse);
		
		//Sauvegarde du plateau
		if(reponse == 'o'){
			char *nomFichier = (char *)malloc(sizeof(char) * 21);
			printf("Donnez un nom au fichier (max 20. caractères et seule les lettres/chiffres sont autorisés).\n");
			scanf(" %s", nomFichier);
			if(FichierExiste(nomFichier)){
				printf("Ce nom existe déjà, souhaitez-vous écraser la sauvegarde ? (o/n) \n");
				scanf(" %s", &reponse);
				if(reponse == 'o'){
					SauvJeu(nomFichier, A, DIMTAQUIN);
				}else{
					free(nomFichier);
					nomFichier = AttributionNom();
					if(nomFichier != NULL){
						printf("Nous avons attribuer le nom %s\n", nomFichier);		
						SauvJeu(nomFichier, A, DIMTAQUIN);
					}
				}
			}
		}

	}
	
	Liberation(Mid, DIMTAQUIN);
	free(sauv);
	free(dim);
	dim = NULL;
	
	return 0;
}
