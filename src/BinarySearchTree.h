#ifndef BST_H
#define BST_H

int compare(const CGameBoard & p1, const CGameBoard & p2);

class BinarySearchTree {

public:

	struct tree_node
	{
		CGameBoard config;
		tree_node * left;
		tree_node * right;
	};

	BinarySearchTree(void);
	~BinarySearchTree(void);

	bool isEmpty(void);

	bool add(const CGameBoard & p);

private:
	tree_node * root;
	bool add(tree_node ** node, const CGameBoard & p);
};

#endif
