#ifndef BST_H
#define BST_H

int compare(const CGameBoard & p1, const CGameBoard & p2);

template <typename T>
class BinarySearchTree
{

public:

	struct tree_node
	{
		T data;
		tree_node * left;
		tree_node * right;
	};

	BinarySearchTree(void) : root(NULL) { }

	~BinarySearchTree(void)
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

	bool isEmpty(void)
	{
		return (NULL == this->root);
	}

	/*
	 * Ajoute une configuration dans un ABR s'il n'est pas deja present
	 * Renvoi 1 si l'ajout est effectué
	 * Renvoi 0 si l'ajout n'est pas effectué
	 */
	bool add(const T & p)
	{
		return this->add(&(this->root), p);
	}

private:

	bool add(tree_node ** node, const T & p)
	{
		if (NULL == *node) {

			*node = new tree_node;

			(*node)->left	= NULL;
			(*node)->right	= NULL;
			(*node)->data = p;

			return true;

		} else {

			int comparaison = compare(p, (*node)->data);

			if (comparaison == 0) {
				return false;
			} else if (comparaison == 1) {
				return this->add(&((*node)->right), p);
			} else {
				return this->add(&((*node)->left), p);
			}
		}
	}


	tree_node * root;
};

#endif
