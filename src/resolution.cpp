#include "pre.h"

/*
 * Teste si une configuration de parking est 'strictement superieure' a une autre configuration
 * selon l ordre defini
 */
int superieur(CGameBoard p1, CGameBoard p2) {

	int nb_vehicules = p1.vehicules.size();
	int i;

	for (i = 0; i < nb_vehicules; i++) {
		if ( p1.vehicules[i]->position.abs > p2.vehicules[i]->position.abs ) {
			return 1;
		} else {

			if ( p1.vehicules[i]->position.abs == p2.vehicules[i]->position.abs ) {
				if ( p1.vehicules[i]->position.ord > p2.vehicules[i]->position.ord ) {
					return 1;
				} else {
			 		if (p1.vehicules[i]->position.ord != p2.vehicules[i]->position.ord) {
						return 0;
					}
			 	}
			} else {
				return 0;
			}
		}
	}

	return 0;
}

/*
 * Teste si une configuration de parking est 'egale' a une autre configuration
 * selon l ordre defini
 */
int egal(CGameBoard p1, CGameBoard p2){

	int nb_vehicules = p1.vehicules.size();
	int i;

	for (i = 0; i < nb_vehicules; i++) {
		if ( p1.vehicules[i]->position.abs != p2.vehicules[i]->position.abs || p1.vehicules[i]->position.ord != p2.vehicules[i]->position.ord ) {
			return 0;
		}
	}

	return 1;
}

/*
 * Ajoute une configuration dans un ABR s'il n'est pas deja present
 * Renvoi 1 si l'ajout est effectué
 * Renvoi 0 si l'ajout n'est pas effectué
 */
int ajout(t_ABR *pa, CGameBoard p){
	if (*pa == NULL) {

		*pa = new t_Noeud;
		(*pa)->f_gauche=NULL;
		(*pa)->f_droit=NULL;

		for (unsigned int k = 0; k < p.vehicules.size(); k++) {
			(*pa)->config.vehicules.push_back(new CGameBlock(p.vehicules[k]->position, p.vehicules[k]->size, p.vehicules[k]->axis));
		}

		return 1;

	} else {

		if (egal(p,(*pa)->config)) {
			return 0;
		}

		if ( superieur(p,(*pa)->config) ){
			return ajout(&((*pa)->f_droit), p);
		} else {
			return ajout(&((*pa)->f_gauche), p);
		}
	}
}

/*
 * Enleve le premier element de la file
 */
void defiler(t_file *pF){
	t_file aux = *pF;
	*pF = (*pF)->suivant;
	//free(aux->config.vehicules); // FIXME memory leak
	free(aux);
}

void free_arbre(t_ABR arbre){
    t_ABR fg, fd;
    if (arbre != NULL){
        fg = arbre->f_gauche;
        fd = arbre->f_droit;

        free(arbre);

        arbre = NULL;

        if(fg != NULL) {
			free_arbre(fg);
		}

        if(fd != NULL) {
			free_arbre(fd);
		}
    }
}

void free_file(t_file file){
    if (file != NULL){
        while (file->suivant != NULL){
            defiler(&file);
        }
    }
}
