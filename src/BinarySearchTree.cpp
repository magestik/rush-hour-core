#include "pre.h"

BinarySearchTree::BinarySearchTree(void)
: root(NULL)
{

}

BinarySearchTree::~BinarySearchTree(void)
{
	// TODO free :
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

bool BinarySearchTree::isEmpty(void)
{
	return (NULL == this->root);
}

/*
 * Ajoute une configuration dans un ABR s'il n'est pas deja present
 * Renvoi 1 si l'ajout est effectué
 * Renvoi 0 si l'ajout n'est pas effectué
 */
bool BinarySearchTree::add(const CGameBoard & p) {
	return this->add(&(this->root), p);
}

bool BinarySearchTree::add(tree_node ** node, const CGameBoard & p) {
	if (NULL == *node) {

		*node = new tree_node;

		(*node)->left	= NULL;
		(*node)->right	= NULL;
		(*node)->config = p;

		return true;

	} else {

		int comparaison = compare(p, (*node)->config);

		if (comparaison == 0) {
			return false;
		} else if (comparaison == 1) {
			return this->add(&((*node)->right), p);
		} else {
			return this->add(&((*node)->left), p);
		}
	}
}

/*
 * Compare deux plateaux selon un ordre défini (pour l'ABR)
 * retourne 0 si ils sont identiques
 * retourne 1 si le premier est supérieur
 * retourne -1 si le deuxième est supérieur
 */
int compare(const CGameBoard & p1, const CGameBoard & p2) {

	int nb_vehicules = p1.vehicules.size();

	for (int i = 0; i < nb_vehicules; i++) {

		if (p1.vehicules[i].position.m_x > p2.vehicules[i].position.m_x) {

			return 1;

		} else if (p1.vehicules[i].position.m_x < p2.vehicules[i].position.m_x) {

			return -1;

		} else {

			if (p1.vehicules[i].position.m_y > p2.vehicules[i].position.m_y) {

				return 1;

			} else if (p1.vehicules[i].position.m_y != p2.vehicules[i].position.m_y) {

				return -1;

			}
		}
	}

	return 0;
}
