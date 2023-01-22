#ifndef MATRICE
#define MATRICE

typedef struct {
	int **M;//Plateau de jeu
	unsigned int *zero;//Place du 0 (vide)
}Plateau;

void AfficheMatrice(int **Matrice, unsigned int nbligne, unsigned int nbcolonne);
Plateau* PlateauId(unsigned int nbligne, unsigned int nbcolonne);
int** MatriceId(unsigned int nbligne, unsigned int nbcolonne);
void Liberation(int **M, unsigned int nbligne);
void echange(Plateau *A, unsigned int i1, unsigned j1, unsigned int i2, unsigned int j2);
int estDifferente(int **M1, int **M2, unsigned int dim);

#endif
