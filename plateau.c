#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "matrice.h"
#include "plateau.h"
	
void PlateauRandom(Plateau *A, unsigned int dim){
//Fonction permettant de mélanger un plateau aléatoirement

	if(A == NULL){
		printf("Problème initialisation du plateau de jeu.\n");
		exit(EXIT_FAILURE);
	}else{
		srand( time(NULL) );
		int random = 100 + ( rand() % 9000 );//On prévoit au moins 100 changements
		while(random >0){
			random --;
			int direction = rand() %4;
			int i = A->zero[0];
			int j = A->zero[1];
			/* 0 = gauche; 1 = droite; 2 = haut; 3 = bas*/
			
			//gestion des bords :
			if(direction == 0 && A->zero[1] == 0){
				direction = 1;
			}else if(direction == 1 && A->zero[1] == (dim-1)){
				direction = 0;
			}else if(direction == 2 && A->zero[0] == 0){
				direction = 3;
			}else if(direction == 3 && A->zero[0] == (dim-1)){
				direction = 2;
			}
							
			//Echange:	
			if(direction == 0){ //interprété par gauche
				echange(A, i, j, i, j-1);
			}else if(direction == 1){//interprété par droite
				echange(A, i, j, i, j+1);
			}else if(direction == 2){//interprété par haut
				echange(A, i, j, i-1, j);
			}else{ //bas
				echange(A, i, j, i+1, j);
			}
		}
	}
}
				
void SauvJeu(char *nomFichier, Plateau *A, unsigned int dim){
	
	//On place dans le dossier attribué aux sauvegarde
	char *extension = (char *)malloc(sizeof(char) * 100);
	strcpy(extension, "Sauvegarde/");
	strcat(extension, nomFichier);
	
	//Création du fichier
	FILE *f = fopen(extension, "w+");
	if(f == NULL){
		printf("La sauvegarde a échouée. Erreur ouverture de fichier.\n");
		exit(-1);
	}
	
	unsigned int i,j;
	char *c = malloc(sizeof(char) * 5);
	sprintf(c, "%d \n", dim);//Ligne de la dimension
	fputs(c, f);
	
	//Lignes de la matrice format %d %d ... %d \n
	for(i=0; i<dim; i++){
		for(j=0; j<dim; j++){
			
			sprintf(c, "%d", A->M[i][j]);
			fputs(c, f);
			fputc(' ', f);
		}
		fputc('\n', f);
	}
	//Ligne des indices du 0
	sprintf(c, "%d", A->zero[0]);
	fputs(c, f);
	fputc(' ', f);
	sprintf(c, "%d", A->zero[1]);
	fputs(c, f);
	fputc(' ', f);
	fclose(f);
	printf("Sauvegarde terminée avec succès\n");
}

int FichierExiste(char *nomFichier){
	//On cherche la sauvegarde dans le dossier approprié
	char extension[100] = "Sauvegarde/";
	strcat(extension, nomFichier);
	FILE *f = fopen(extension, "r");
	if(f != NULL){
		fclose(f);
		return 1;
	}
	return 0;
}

char* AttributionNom(){
	//On limite le nombre de sauvegarde générée à 100 (Jeu0.twt à Jeu99.txt)
	char *nomFichier =(char *) malloc(sizeof(char) * 10);
	strcpy(nomFichier, "Jeu0.txt");
	if(!FichierExiste(nomFichier)){
		return nomFichier;
	}
	
	int i = 1;
	while(FichierExiste(nomFichier) && i<100){
		//On cherche un numéro qui n'existe pas déjà
		sprintf(nomFichier, "Jeu%d.txt", i);
		i++;
	}
	
	if(i == 100){
		printf("Désolé il n'y a plus de place nous ne pouvons sauvegarder votre partie.\n");
		return NULL;
	}
	return nomFichier;
}


Plateau* Recuperation(char *nomFichier, int *dim){
	//On cherche la sauvegarde dans le dossier approprié
	char extension[100] = "Sauvegarde/";
	strcat(extension, nomFichier);
	FILE *f= fopen(extension, "r");
	if(f == NULL){
		printf("Erreur lors de la récupération du fichier.\n");
		return NULL;
	}
	
	//On récupère les élément 1 à 1 en commençant par la dimension
	fseek(f, 0, SEEK_SET);
	char *recuperation = (char *)malloc(sizeof(char) * 3);
	recuperation = fgets(recuperation, 3, f);
	*dim = atoi(recuperation);
	printf("dimension: %d\n", *dim);
	
	//Attribution d'espace mémoire pour le Plateau
	Plateau *A = (Plateau *)malloc(sizeof(Plateau));
	A->M = (int **)malloc(sizeof(int *) * (*dim));
	for(int i=0; i<*dim; i++){
		A->M[i] = (int *)malloc(sizeof(int) * (*dim));
	}
	A->zero = (unsigned int *)malloc(sizeof(unsigned int) * 2);
	/***Verif pour erreur***/
	A->zero[0] = -1;
	A->zero[1] = -1;
	/**********************/
	int i,j,b;
	i=0;
	j=0;
	b=0;//Variable qui permet de savoir si récupération est vide
	char *Psurc = (char *)malloc(sizeof(char));//Pointeur car nécessaire pour strcat
	while( (Psurc[0] = fgetc(f)) != EOF){
		if( (Psurc[0] == ' ') || (Psurc[0] == '\n') ){
			//Si on a fini de récuperer un nombre dans recuperation
			if(b && (i != (*dim)) ){
				//Si on a des valeurs dans recuperation et qu'on récupère des valeurs pour la matrice
				A->M[i][j] = atoi(recuperation);
				j++;
				if(j == (*dim)){
					//On passe à la ligne suivante
					j = 0;
					i++;
				}
				b = 0;
			}else if(i == (*dim) && b){
				//On a récupéré toutes les valeurs de la matrice on récupère maintenant les indices du 0
				if(A->zero[0] == (unsigned int) -1){
					A->zero[0] = atoi(recuperation);
					b = 0;
				}else if(A->zero[1] == (unsigned int) -1){
					A->zero[1] = atoi(recuperation);
					b = 0;
				}
			}
			//On "vide" notre chaine	
			strcpy(recuperation, "\0");	
		}else{
			b = 1;
			strcat(recuperation, Psurc);
		}	
	}
	return A;
}



void Jeu(Plateau *A, int dim, char mouvement){
	int i,j;
	i = (int)A->zero[0];
	j = (int)A->zero[1];
	//On verifie la saisie de l'utilisateur
	if( (j == 0 && mouvement == 'g') || (j == (dim-1) && mouvement == 'd') || (i == 0 && mouvement == 'h') || (i == (dim-1) && mouvement == 'b')){
		printf("Mouvement non valide, veuillez recommencer.\n");
	}else{
		//On échange les valeurs correspondantes selon le choix de l'utilisateur
		int temp;
		if(mouvement == 'g'){
			temp = A->M[i][j];
			A->M[i][j] = A->M[i][j-1];
			A->M[i][j-1] = temp;
			A->zero[1]--;
		}else if(mouvement == 'd'){
			temp = A->M[i][j];
			A->M[i][j] = A->M[i][j+1];
			A->M[i][j+1] = temp;
			A->zero[1]++;
		}else if(mouvement == 'h'){
			temp = A->M[i][j];
			A->M[i][j] = A->M[i-1][j];
			A->M[i-1][j] = temp;
			A->zero[0]--;
		}else if(mouvement == 'b'){
			temp = A->M[i][j];
			A->M[i][j] = A->M[i+1][j];
			A->M[i+1][j] = temp;
			A->zero[0]++;
		}
	}
}
