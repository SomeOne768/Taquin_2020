#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int **M;
	unsigned int *zero;
}Plateau;

Plateau* Recuperation(char *nomFichier, int *dim){
	FILE *f= fopen(nomFichier, "r");
	if(f == NULL){
		printf("Erreur lors de la récupération du fichier.\n");
		return NULL;
	}
	
	fseek(f, 0, SEEK_SET);
	char *recuperation = (char *)malloc(sizeof(char) * 3);
	recuperation = fgets(recuperation, 3, f);
	*dim = atoi(recuperation);
	printf("dim: %d\n", *dim);
	
	Plateau *A = (Plateau *)malloc(sizeof(Plateau));
	A->M = (int **)malloc(sizeof(int *) * (*dim));
	for(int i=0; i<*dim; i++){
		A->M[i] = (int *)malloc(sizeof(int) * (*dim));
	}
	
	A->zero = (unsigned int *)malloc(sizeof(unsigned int) * 2);
	A->zero[0] = -1;
	A->zero[1] = -1;
	int i,j,b;i=0;j=0;b=0;
	char *Psurc = (char *)malloc(sizeof(char));
	while( (Psurc[0] = fgetc(f)) != EOF){
		if( (Psurc[0] == ' ') || (Psurc[0] == '\n') ){
			if(b && (i != (*dim)) ){
				A->M[i][j] = atoi(recuperation);
				j++;
				if(j == (*dim)){
					j = 0;
					i++;
				}
				b = 0;
			}else if(i == (*dim) && b){
				if(A->zero[0] == (unsigned int) -1){
					A->zero[0] = atoi(recuperation);
					b = 0;
				}else if(A->zero[1] == (unsigned int) -1){
					A->zero[1] = atoi(recuperation);
					b = 0;
				}
			}				
			strcpy(recuperation, "\0");	
		}else{
			b = 1;
			strcat(recuperation, Psurc);
		}	
	}
	return A;
}

void TrouverChiffre(int **M, int *i, int *j, int dim, int chiffreATrouver){
	int ligne = 0;
	int colonne = 0;
	int continuer = 1;
	while( ligne != dim && continuer){
		if(M[ligne][colonne] == chiffreATrouver){	
				*i = ligne;
				*j = colonne;
				continuer = 0;				
		}
		if( colonne == dim -1){
			ligne++;
			colonne = 0;
		}else{
			colonne++;
		}
	}
}
	


void AfficheMatrice(int **M, unsigned int nbligne, unsigned int nbcolonne){
	unsigned int i,j;
	for(i=0; i<nbligne; i++){
		printf("|");
		for(j=0; j<nbcolonne; j++){
			if(M[i][j]<10){
				printf("  %d |", M[i][j]);
			}else{
				printf(" %d |", M[i][j]);
			}
		}
		printf("\n");
	}
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

void Placement0(Plateau *A, int dim, int i, int j){//i et j <dim

	while(A->M[i][j] != 0){
	//Pour faciliter la résolution après, on se deplace unique sur un axe tant que la position n'est pas atteinte
		
		if(A->zero[0] != i){
			//Deplacement sur i:
			if(A->zero[0] < i){
				//Aller en bas
				echange(A, A->zero[0], A->zero[1], A->zero[0] + 1, A->zero[1]);
			}else if(A->zero[0] > i){
				//Aller en haut
				echange(A, A->zero[0], A->zero[1], A->zero[0] - 1, A->zero[1]);
			}
		}else{
			//on se deplace d'abord sur j:
			if(A->zero[1] < j){
				//Aller à droite
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1] + 1);
			}else if(A->zero[1] > j){
				//Aller à gauche
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1] - 1);
			}	
			
		}
		
	}
}
	

void PlacementHG(Plateau *A, int dim, int k, int i, int j){
	int i0,j0,tempi,tempj;
	i0 = (k-1)/dim;
	j0 = (k-1)%dim;
	//int it = 15;

	while((A->M[i0][j0] != k)){//it--;
	
		//On se place à droite de la cible
		if(i != A->zero[0] || j!=(A->zero[1] -1)){
			if(i != A->zero[0]){//Pas aligné sur i
				if(j!= dim-1){//on peut se coller au mur et sapprocher sans toucher de case placée
					Placement0(A, dim, A->zero[0], dim-1);
					Placement0(A, dim, i, j+1);
				}else{//On peut s'approcher sans risquer de toucher une piece placée
					Placement0(A, dim, A->zero[0], j-1);
					Placement0(A, dim, i, j-1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}
			}else{//Cas aligné sur i
				if(j > A->zero[1]){// se trouve à droite
					Placement0(A, dim, A->zero[0], j-1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}else{//se trouve à gauche
					Placement0(A, dim, A->zero[0], j+1);
				}
			}
		}
				
		
		/*******************************************************************/		

		
		//Si on a besoin de ramener la cible sur la gauche en faisant attention à ne pas toucher les cases placées
		if(j > j0 && i>=i0){
			if(i == (dim-1)){
				//On doit passer par le haut
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				//on est à gauche
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
				
			}else{
				//On doit passer par le bas pour ne pas risquer de modifier les pieces déjà placées
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				//on est à gauche
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}//La Cible a été déplacé à gauche on est à droite d'elle
		
		//Si on a besoin de ramener la cible vers le haut en faisant attention de ne pas toucher les cases placées
		else if(i > i0 && j == j0){
			//H puis G
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			//La Cible a été déplacé en haut on est en bas d'elle
			//on se replace sur sa droite
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			
		}//La Cible a été déplacé en haut on est à droite d'elle
		
		//Si on a besoin de ramener la cible vers le bas
		else if(i < i0){
			//B puis G
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			//On est en haut d'elle
			//On se remet sur sa droite
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);			
		}//La Cible a été déplacé en bas on est à droite d'elle
		
		
	
		//si on a besoin de ramener la cible vers la droite
		else if(j < j0){//Comme on va se remettre à sa droite on la decale 2*
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
		}
	}
	
}

void PlacementHFIN(Plateau *A, int dim, int k, int i, int j){
	//A ce stade on sera collé au mur (0)
	
	int ik = (k)/dim;
	int jk = (k-1)%dim;
	int tempi,tempj;
	while(A->M[ik][jk] != k){
	
		//on se colle a gauche :
		if(i != A->zero[0] || j != A->zero[1] + 1){

			if(i != A->zero[0]){
			//Pas meme ligne
						
				if(j < A->zero[1]){
					//sur la gauche	
					Placement0(A, dim, A->zero[0], dim-1);
					Placement0(A, dim, i, dim-1);
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;	
				}else if(j>A->zero[1]){
					Placement0(A, dim, i, A->zero[1]);
					Placement0(A, dim, i, j-1);
				}else{
				//On est sur la même colonne
					if(i == A->zero[0]+1){
						tempi = A->zero[0];
						tempj = A->zero[1];
						echange(A, A->zero[0], A->zero[1], i, j);
						i = tempi;
						j = tempj;
						if(A->M[ik][jk] != k){
							if(A->zero[1] != dim-1){
								echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
								echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
								tempi = A->zero[0];
								tempj = A->zero[1];
								echange(A, A->zero[0], A->zero[1], i, j);
								i = tempi;
								j = tempj;	
							}else{
								echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
								echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
							}
						}
					}else{
						Placement0(A, dim, A->zero[0]+1, A->zero[1]);
						Placement0(A, dim, A->zero[0], 0);
						Placement0(A, dim, i, A->zero[1]);
						Placement0(A, dim, i, j-1);
					}
				
				}
			}else{
				if(j>A->zero[1]){
					Placement0(A, dim, i, j-1);
				}else{
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}
			}
		}//On est a gauche de la cible
		
		
		/******************************************/
		

		//1 besoin de l'enmerner en haut
		if(i != ik){
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
		}else if(j!= dim -1){
		//2 besoin => droite
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			if(j!= dim-1){//Necessaire car on la ramene a gauche parfosi pendant le placement
				//2eme fois on est encore à sa gauche
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}

	}
	//On place le 0 a gauche de k pour la suite
	Placement0(A, dim, A->zero[0], dim-2);
	Placement0(A, dim, i, dim-2);
}	
				
				
				
				
				
				

void PlacementHD(Plateau *A, int dim, int k, int i, int j){
	//Cas ou il ne reste que 2 cases à mettre sur la ligne
	//Le 0 sera forcement en (icible, dim-2)
	int tempi, tempj;
	int ik = (k-1)/dim;
	int jmur = dim-1;
	int i0=0, j0 = 0;
	while(A->M[ik][jmur] != k){
		
		//On se place à gauche de la cible
		//Pour cela on a accès qu'aux cases en dessous du 0 initial et aux 2 dernieres colonnes
		if(i != A->zero[0] || j != A->zero[1]+1){
			//Aligné avec i:
			if(i == A->zero[0]){
				if(j < A->zero[1]){
					Placement0(A, dim, A->zero[0], j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}else{
					Placement0(A, dim, A->zero[0], j-1);
				}
				
			}else if(i < A->zero[0]){
				//Au dessus
				// 1 - au dessus et je peux pas me mettre à gauche
				// 2 - aligné je dois faire attention
				if(j != (dim-1)){
					Placement0(A, dim, A->zero[0], j+1);
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}else{
					Placement0(A, dim, i, A->zero[1]);
				}
			}else{
				if(j != (dim-1)){
					Placement0(A, dim, A->zero[0], (dim-1));
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;
				}else{
					Placement0(A, dim, i, j-1);
				}
			}
		}
		
		
		
		
		//On met k dans (i0+1, j+1)
		if(i != (i0+1) || j != (j0+1)){
			if(i == (dim-1)){
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}else if(j < (dim-1)){
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+2);
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}else if(i > i0){
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}else if(i  == i0){
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}
	}
	
}
	
void ajuster(Plateau *A, int dim){
	//Le k-1 est en haut et le k a notre droite
	echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
	echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);//k est descendu ici
	echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);//k-1 est a droite
	echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);//on est entre les deux
	echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);//k+1 est monté et ils sont collés. 0 est en bas de k+1
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
	echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);//k placé
	echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);//k+1 placé
}
		
void Placer2DerniereLigne(Plateau *A, int dim, int k){
	//Etape 1 on met k+dim dans le bord inferieur droit
	int i,j,tempi,tempj;
	TrouverChiffre(A->M, &i, &j, dim, k+dim);
	while(A->M[dim-1][dim-1] != (k+dim)){
		
		//on se colle a gauche :
		if(i != A->zero[0] || j != A->zero[1] + 1){
			if(i != A->zero[0]){//Pas meme ligne
				if(j != dim-1){
					Placement0(A, dim, A->zero[0], dim-1);
					Placement0(A, dim, i, dim-1);
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;	
				}else{
					Placement0(A, dim, A->zero[0], j-1);
					Placement0(A, dim, i, j-1);
				}
			}else{
				//colonne differente
				if(j > A->zero[1]){
					Placement0(A, dim, A->zero[0], j-1);
				}else{
					Placement0(A, dim, i, j+1);
					tempi = A->zero[0];
					tempj = A->zero[1];
					echange(A, A->zero[0], A->zero[1], i, j);
					i = tempi;
					j = tempj;						
				}
			}
		}//On est a gauche de la cible
		
		//il est pas au bord inferieur:
		if(i != dim-1){
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
		}else if(j != dim-1){//Il est pas au bord droit
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			if(j != dim-1){//Pour le placement initial on le depalce parfois donc obligé
			//On est tout de même à sa gauche
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}
	}
	
	//Ici k+dim est dans le bord inférieur droit
	///on place maintenant k a gauche de k+dim
	TrouverChiffre(A->M, &i, &j, dim, k);
	while(A->M[dim-1][dim-2] != k){
		
		//on se colle a gauche. Cas particulier: on a acces qu'au 2 derniere ligne:
		if(i != A->zero[0] || j != A->zero[1] + 1){
			
			if(j>A->zero[1]){
				Placement0(A, dim, i, A->zero[1]);
				Placement0(A, dim, i, j-1);
			}else if(j<A->zero[1]){
				if(i == A->zero[0]){//meme ligne
					Placement0(A, dim, A->zero[0], j+1);	
				}else{
					Placement0(A, dim, A->zero[0], dim-2);
					Placement0(A, dim, i, dim-2);
					Placement0(A, dim, i, j+1);
				}
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}else{//même colonne => pas même ligne
				Placement0(A, dim, A->zero[1], dim-2);
				Placement0(A, dim, i, dim-2);
				Placement0(A, dim, i, j+1);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}//On est a gauche de la cible
		if(k==10){AfficheMatrice(A->M, dim, dim);printf("\n");}
	
		//il est pas au bord inferieur:
		if(i != dim-1){
			if(j!= dim-1){//Pour pas redeplacer k+dim
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}else{//on echange et on deplace les deux
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
			}//Les deux sont dans le ocin inférieur				
		}else if(j != dim-2){//Il n'est pas au coller a k+dim
			echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
			echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
			tempi = A->zero[0];
			tempj = A->zero[1];
			echange(A, A->zero[0], A->zero[1], i, j);
			i = tempi;
			j = tempj;
			if(j != dim-2){//Pour le placement initial on le depalce parfois donc obligé
			//On est tout de même à sa gauche
				echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
				echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
				tempi = A->zero[0];
				tempj = A->zero[1];
				echange(A, A->zero[0], A->zero[1], i, j);
				i = tempi;
				j = tempj;
			}
		}
	}
}
		
		
void ajusterbas(Plateau *A, int dim, int k){
	//A ce stade k et k+dim sont tout en bas a droite
	int jk = (k-1) % dim; 
	Placement0(A, dim, A->zero[0],dim-3);
	Placement0(A, dim, dim-1, dim-3);//on est collé à ik
	
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);//ik et ik+dim on été deplacé à gauche et le 0 est au coin
	//Tant qu'ils ne sont pas sur la bonne colonne on les ramène a gauche
	while(A->M[dim-1][jk] != k){
		//le 0 est forcément à droite
		echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
		echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
		//Je suis a gauche de k
		
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);//ik et ik+dim on été deplacé à gauche et le 0 est au coin
		
	}
	
	//il ne reste plus qu'a pivoter:
	//Le 0 est a droite
	
	echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
	echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
	echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
	//Ok
	
}
		
void derniere4Cases(Plateau *A, int dim){
	Placement0(A, dim, dim-1,dim-1);
	int gauche, haut, hautgauche;
	gauche = (dim * dim) - 1;
	haut = dim * (dim - 1);
	hautgauche = haut-1;
	
	while(A->M[dim-1][dim-2] !=  gauche || A->M[dim-2][dim-1] != haut || A->M[dim-2][dim-2] != hautgauche){
		//on tourne en rond
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]-1);
		echange(A, A->zero[0], A->zero[1], A->zero[0]-1, A->zero[1]);
		echange(A, A->zero[0], A->zero[1], A->zero[0], A->zero[1]+1);
		echange(A, A->zero[0], A->zero[1], A->zero[0]+1, A->zero[1]);
	}
}
		
		
void Resolution(Plateau *A, int dim){
	int k;
	int *it = malloc(sizeof(int));
	int *jt = malloc(sizeof(int));
	int reste2ligne = 0;
	for(k=1; k<=((dim * dim) - 2 - dim); k++){
		
		if(k > ( (dim * dim) - (2 * dim))){
			reste2ligne = 1;
		}
		
		printf("POUR %d\n", k);
		
		if( (A->M[(int) (k-1)/dim][(k-1)%dim] != k) && (!reste2ligne) ){
		
			if((k%dim) != 0){ 	
				TrouverChiffre(A->M, it, jt, dim, k);
				PlacementHG(A, dim, k, *it, *jt);
			}else{
				TrouverChiffre(A->M, it, jt, dim, k);

				PlacementHFIN(A, dim, k, *it, *jt);
				if(A->M[(int) (k-1)/dim][(k-1)%dim] != k){
					ajuster(A, dim);
				}
			}
			
		}else if(reste2ligne){
			AfficheMatrice(A->M, dim, dim);printf("\n");
			Placer2DerniereLigne(A, dim, k);
			ajusterbas(A, dim, k);
		}
		AfficheMatrice(A->M, dim, dim);printf("\n");
		printf("fin du  %d\n", k);
		
	}
	derniere4Cases(A, dim);
	AfficheMatrice(A->M, dim, dim);printf("END\n");
}
	

int main (){
	int *dim = malloc(sizeof(int));
	Plateau *A = Recuperation("Sauvegarde/test.txt", dim);
	AfficheMatrice(A->M, *dim, *dim);printf("On commence la résolution\n");
	
	Resolution(A, *dim);

	return 0;
}
