#include "pre.h"

/*
 * Renvoie un tableau d'entiers representant l'occupation du parking
 * si case vide : 0
 * si case occupée : numero du véhicule
 */
void get_parking_occupation(CGameBoard parking, int *tableau) {

	for (int i = 0; i < HEIGHT * WIDTH; i++) {
		tableau[i] = 0;
	}

	for (unsigned int i = 0; i < parking.vehicules.size(); i++) {
		// numerotation de la case reference.
		tableau[parking.vehicules[i]->position.ord*6+parking.vehicules[i]->position.abs] = 1+i;
		// numerotation de la case suivante en fonction de l'axe
		tableau[(parking.vehicules[i]->position.ord+parking.vehicules[i]->axis)*6+parking.vehicules[i]->position.abs+1-parking.vehicules[i]->axis] = 1+i;
		// numerotation de la case suivante OU de la 2e case qui suit en fonction de la taille
		tableau[(parking.vehicules[i]->position.ord+(parking.vehicules[i]->size-1)*parking.vehicules[i]->axis)*6+parking.vehicules[i]->position.abs+(parking.vehicules[i]->size-1)*(1-parking.vehicules[i]->axis)] = 1+i;
	}

	//return tableau;
}

/*
 * Retourne :
 * 1 si mouvement impossible
 * 0 si mouvement possible
 */
int is_move_possible(int mvt, int num_vehicule, CGameBoard parking){
	int cases_occ[HEIGHT * WIDTH];

	get_parking_occupation(parking, cases_occ);

	int newAbs;
	int newOrd;
	int largeur = (parking.vehicules[num_vehicule]->size-1) * (1 - parking.vehicules[num_vehicule]->axis); // largeur
	int hauteur = (parking.vehicules[num_vehicule]->size-1) * parking.vehicules[num_vehicule]->axis; // hauteur

	if(mvt != 1 && mvt != 2) {
		return 1;
	}

	if (mvt == 2) { // 2 : droite ou bas
		newAbs = parking.vehicules[num_vehicule]->position.abs + (1 - parking.vehicules[num_vehicule]->axis); // horizontal
		newOrd = parking.vehicules[num_vehicule]->position.ord + parking.vehicules[num_vehicule]->axis; // vertical
	} else { // 1 : gauche ou haut
		newAbs = parking.vehicules[num_vehicule]->position.abs - (1 - parking.vehicules[num_vehicule]->axis); // horizontal
		newOrd = parking.vehicules[num_vehicule]->position.ord - parking.vehicules[num_vehicule]->axis; // vertical
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
void move(int mvt, unsigned int num_vehicule, CGameBoard parking) {
	if (num_vehicule < parking.vehicules.size()) {
		parking.vehicules[num_vehicule]->position.abs += (1 - parking.vehicules[num_vehicule]->axis)*mvt;
		parking.vehicules[num_vehicule]->position.ord += parking.vehicules[num_vehicule]->axis*mvt;
	}
}

/*
 * Charge le prochain niveau du fichier
 */
void load_next_level() {
	//free(parking_actuel->vehicule);
	//free(parking_actuel->position);
	free(parking_actuel);

	parking_actuel = read_level_file();
}
