#ifndef STRUCTURES_H
#define STRUCTURES_H

#define HEIGHT 6
#define WIDTH 6

typedef struct {
	short abs;
	short ord;
} t_position;

typedef struct {
	short taille;
	short axe; // horizontal = 0 ; vertical = 1
} t_vehicule;

typedef struct {
	int nb_vehicules;
	t_vehicule * vehicule;
	t_position * position;
} t_parking;

typedef struct {
	short voiture;
	short deplacement;
} t_mouvement;

typedef t_mouvement * t_chemin;

typedef struct sMaillon {
	t_parking config;
	t_chemin chemin;
	struct sMaillon *suivant;
} t_Maillon;

typedef t_Maillon * t_file;

typedef struct sNoeud {
	t_parking config;
	struct sNoeud *f_gauche;
	struct sNoeud *f_droit;
} t_Noeud;

typedef t_Noeud * t_ABR; 

#endif
