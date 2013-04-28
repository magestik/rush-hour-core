#ifndef RESOLUTION_H
#define RESOLUTION_H

/* ABR */

class ABR_Node {
public:
	ABR_Node(void) : f_gauche(NULL), f_droit(NULL) { }

	CGameBoard config;
	ABR_Node *f_gauche;
	ABR_Node *f_droit;
};

class ABR {
public:
	ABR(void);
	~ABR(void);
	bool add(const CGameBoard & p);

private:
	ABR_Node * root;
	bool add(ABR_Node ** node, const CGameBoard & p);
	int compare(const CGameBoard & p1, const CGameBoard & p2);
};


/* File */

typedef struct sMaillon {
	CGameBoard config;
	t_chemin chemin;
	struct sMaillon *suivant;
} t_Maillon;

typedef t_Maillon * t_file;

void defiler(t_file *pF);
void free_file(t_file file);

#endif
