#include "rush.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "level.h"

#ifdef INTERFACE_CONSOLE
	#include "console.h"
#endif

#ifdef INTERFACE_GRAPHIQUE
	#include "graphique.h"
#endif

t_parking *parking_actuel;

int nb_coups;

/*
 * Renvoie un tableau d'entiers representant l'occupation du parking
 * si case vide : 0
 * si case occupée : numero du véhicule
 */
int *tab_positions(t_parking parking) {
	int i;
	int * tableau = malloc(sizeof(int) * HEIGHT * WIDTH);

	for (i = 0; i < HEIGHT * WIDTH; i++) {
		tableau[i] = 0;
	}

	for (i = 0; i < parking.nb_vehicules; i++) {
		// numerotation de la case reference.
		tableau[parking.position[i].ord*6+parking.position[i].abs] = 1+i;
		// numerotation de la case suivante en fonction de l'axe
		tableau[(parking.position[i].ord+parking.vehicule[i].axe)*6+parking.position[i].abs+1-parking.vehicule[i].axe] = 1+i;
		// numerotation de la case suivante OU de la 2e case qui suit en fonction de la taille
		tableau[(parking.position[i].ord+(parking.vehicule[i].taille-1)*parking.vehicule[i].axe)*6+parking.position[i].abs+(parking.vehicule[i].taille-1)*(1-parking.vehicule[i].axe)] = 1+i;
	}

	return tableau;
}

/*
 * Retourne :
 * 1 si mouvement impossible
 * 0 si mouvement possible
 */
int is_move_possible(int mvt, int num_vehicule, t_parking parking){
	int * cases_occ = tab_positions(parking);

	int newAbs;
	int newOrd;
	int largeur = (parking.vehicule[num_vehicule].taille-1) * (1 - parking.vehicule[num_vehicule].axe); // largeur
	int hauteur = (parking.vehicule[num_vehicule].taille-1) * parking.vehicule[num_vehicule].axe; // hauteur

	if(mvt != 1 && mvt != 2) {
		return 1;
	}

	if (mvt == 2) { // 2 : droite ou bas
		newAbs = parking.position[num_vehicule].abs + (1 - parking.vehicule[num_vehicule].axe); // horizontal
		newOrd = parking.position[num_vehicule].ord + parking.vehicule[num_vehicule].axe; // vertical
	} else { // 1 : gauche ou haut
		newAbs = parking.position[num_vehicule].abs - (1 - parking.vehicule[num_vehicule].axe); // horizontal
		newOrd = parking.position[num_vehicule].ord - parking.vehicule[num_vehicule].axe; // vertical
	}

	//Autorisation de sortie du parking
	if(newAbs+largeur > 5 && newOrd==2){
		return 0;
	}

	// On vérifie si la voiture est entierement dans le damier
	if(newAbs < 0 || newOrd < 0 || newAbs+largeur > 5 || newOrd+hauteur > 5){
		return 1;
	}

	// On vérifie si la case la plus en haut, ou la plus à gauche, est occupée
	if ( mvt == 1 && cases_occ[newOrd*6 + newAbs] != 0 ){
		return 1;
	}

	// On vérifie si la case la plus en bas, ou la plus à droite, est occupée
	if( mvt == 2 && cases_occ[(newOrd+hauteur)*6 + (newAbs+largeur)] != 0 ){
		return 1;
	}

	return 0;
}

/*
 * Modifie le parking en effectuant le mouvement MVT au vehicule numero NUM_VEHICULE
 */
void move(int mvt, int num_vehicule, t_parking parking) {
	parking.position[num_vehicule].abs += (1 - parking.vehicule[num_vehicule].axe)*mvt;
	parking.position[num_vehicule].ord += parking.vehicule[num_vehicule].axe*mvt;
}

/*
 * Charge le prochain niveau du fichier
 */
void load_next_level() {
	free(parking_actuel->vehicule);
	free(parking_actuel->position);
	free(parking_actuel);

	parking_actuel = read_level_file();
}

/*
 * Affiche un petit message d'aide sur l'utilisation et quitte
 */
void usage(char *argv0) {
	printf("Usage:\n");
	printf("%s [-l <level file>]\n", argv0);
	exit(1);
}

int main(int argc, char *argv[]) {

	if( argc > 1 ) {
		if( argc != 3 || strcmp(argv[1], "-l") ){
			usage(argv[0]);
		}

		init_level_file(argv[2]);
	} else {
		init_level_file("levels.data"); // default file name
	}

	#ifdef INTERFACE_CONSOLE
		console_init();
	#endif

	#ifdef INTERFACE_GRAPHIQUE
		graphique_init(argc, argv);
	#endif

	close_level_file();

	return 0;
}