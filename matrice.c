#include <stdio.h>
#include <stdlib.h>
#include "matrice.h"


void AfficheMatrice(int **M, unsigned int nbligne, unsigned int nbcolonne){
	unsigned int i,j;
	for(i=0; i<nbligne; i++){
		printf("|");
		for(j=0; j<nbcolonne; j++){
			//On suppose qu'on a au plus 2 chiffres/ nombre et on fait en sorte d'avoir la même largeur
			if(M[i][j] == 0){
				//Si Mi,j == 0 alors c'est la case vide
				printf("    |");
			}else if(M[i][j]<10){
				printf("  %d |", M[i][j]);
			}else{
				printf(" %d |", M[i][j]);
			}
		}
		printf("\n");
	}
}

Plateau* PlateauId(unsigned int nbligne, unsigned int nbcolonne){

	//Attribution de la mémoire pour le Plateau de jeu
	Plateau *A = (Plateau *) malloc(sizeof(Plateau));
	A->M = (int **)malloc(sizeof(int *) * nbligne);
	A->zero = (unsigned int *) malloc(sizeof(int) * 2);
	unsigned int i,j;
	for(i=0; i<nbligne; i++){
		A->M[i] = (int *)malloc(sizeof(int) * nbcolonne);
	}
	
	// Attribution des valeurs
	for(i=0; i<nbligne; i++){
		for(j=0; j<nbcolonne; j++){
			if( i == (nbligne -1) && j == (nbcolonne-1) ){
				//La case vide est réprésentée par le 0
				A->M[i][j] = 0; 
				A->zero[0] = i;
				A->zero[1] = j;
			}else{		
				// i * nb colonne pour prendre en compte les cases du haut
				//j+1 car on démarre à 1 (M0,0 = 1)
				A->M[i][j] = (i * nbcolonne) + (j + 1);
			}
		}
	}
	
	return A;
}

int** MatriceId(unsigned int nbligne, unsigned int nbcolonne){
	int **M = (int **)malloc(sizeof(int *) * nbligne);
	unsigned int i,j;
	
	// Attribution d'espace mémoire
	for(i=0; i<nbligne; i++){
		M[i] = (int *)malloc(sizeof(int) * nbcolonne);
	}
	
	// Attribution des valeurs
	for(i=0; i<nbligne; i++){
		for(j=0; j<nbcolonne; j++){
			if( i == (nbligne -1) && j == (nbcolonne-1) ){
				//La case vide est réprésentée par le 0
				M[i][j] = 0; 
			}else{		
				M[i][j] = (i * nbcolonne) + (j + 1);
			}
		}
	}
	return M;
}


void Liberation(int **M, unsigned int nbligne){
	for(unsigned int i=0; i<nbligne; i++){
		free(M[i]);
		M[i] = NULL;
	}
	free(M);
}

void echange(Plateau *A, unsigned int i1, unsigned j1, unsigned int i2, unsigned int j2){
	//Mise à jour des indices du 0
	if(A->M[i1][j1] == 0){
		A->zero[0] = i2;
		A->zero[1] = j2;
	}else if(A->M[i2][j2] == 0){
		A->zero[0] = i1;
		A->zero[1] = j1;
	}
		
	//Echange des valeurs
	int temp = A->M[i1][j1];
	A->M[i1][j1] = A->M[i2][j2];
	A->M[i2][j2] = temp;
}

int estDifferente(int **M1, int **M2, unsigned int dim){
	unsigned int i,j;
	for(i=0; i<dim; i++){
		for(j=0; j<dim; j++){
			if(M1[i][j] != M2[i][j]){
				//1 élément de la matrice est différent
				return 1;
			}
		}
	}
	//aucun élément différent n'a été trouvé, les deux matrices sont identiques
	return 0;
}
