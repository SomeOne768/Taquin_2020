#ifndef PLATEAU
#define PLATEAU

void PlateauRandom(Plateau *A, unsigned int dim);
void SauvJeu(char *nomFichier, Plateau *A, unsigned int dim);
Plateau* Recuperation(char *nomFichier, int *dim);
void Jeu(Plateau *A, int dim, char mouvement);
int FichierExiste(char *nomFichier);
char* AttributionNom();

#endif
