#ifndef RESOLUTION_H
#define RESOLUTION_H

typedef struct sMaillon {
	CGameBoard config;
	t_chemin chemin;
	struct sMaillon *suivant;
} t_Maillon;

typedef t_Maillon * t_file;

typedef struct sNoeud {
	CGameBoard config;
	struct sNoeud *f_gauche;
	struct sNoeud *f_droit;
} t_Noeud;

typedef t_Noeud * t_ABR;

int superieur(CGameBoard p1, CGameBoard p2);
int egal(CGameBoard p1, CGameBoard p2);
int ajout(t_ABR *pa, CGameBoard p);
void defiler(t_file *pF);
void free_arbre(t_ABR arbre);
void free_file(t_file file);

#endif
