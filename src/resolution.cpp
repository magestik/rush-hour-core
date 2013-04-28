#include "pre.h"

ABR::ABR()
{
	this->root = NULL;
}

ABR::~ABR()
{
	/*
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
    }*/
}

/*
 * Ajoute une configuration dans un ABR s'il n'est pas deja present
 * Renvoi 1 si l'ajout est effectué
 * Renvoi 0 si l'ajout n'est pas effectué
 */
bool ABR::add(const CGameBoard & p) {
	return this->add(&(this->root), p);
}

bool ABR::add(ABR_Node ** node, const CGameBoard & p) {
	if (NULL == *node) {

		*node = new ABR_Node;

		(*node)->f_gauche	= NULL;
		(*node)->f_droit	= NULL;

		(*node)->config = p;

		return true;

	} else {

		int comparaison = this->compare(p, (*node)->config);

		if (comparaison == 0) {
			return false;
		} else if (comparaison == 1) {
			return this->add(&((*node)->f_droit), p);
		} else {
			return this->add(&((*node)->f_gauche), p);
		}
	}
}

/*
 * Compare deux plateaux selon un ordre défini (pour l'ABR)
 * retourne 0 si ils sont identiques
 * retourne 1 si le premier est supérieur
 * retourne -1 si le deuxième est supérieur
 */
int ABR::compare(const CGameBoard & p1, const CGameBoard & p2) {

	int nb_vehicules = p1.vehicules.size();

	for (int i = 0; i < nb_vehicules; i++) {

		if (p1.vehicules[i]->position.abs > p2.vehicules[i]->position.abs) {

			return 1;

		} else if (p1.vehicules[i]->position.abs < p2.vehicules[i]->position.abs) {

			return -1;

		} else {

			if (p1.vehicules[i]->position.ord > p2.vehicules[i]->position.ord) {

				return 1;

			} else if (p1.vehicules[i]->position.ord != p2.vehicules[i]->position.ord) {

				return -1;

			}
		}
	}

	return 0;
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

void free_file(t_file file){
    if (file != NULL){
        while (file->suivant != NULL){
            defiler(&file);
        }
    }
}
