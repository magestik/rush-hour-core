#include "pre.h"

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


/*explicit*/ CGameBoard::CGameBoard(void)
: vehicules()
{
	this->vehicules.reserve(MAX_VEHICULE);
}

/*virtual*/ CGameBoard::~CGameBoard(void)
{

}

void CGameBoard::move(int mvt, unsigned int num_vehicule)
{
	if (num_vehicule < this->vehicules.size()) {
		this->vehicules[num_vehicule].position.m_x += (1 - this->vehicules[num_vehicule].axis) * mvt;
		this->vehicules[num_vehicule].position.m_y += this->vehicules[num_vehicule].axis * mvt;
	}
}

void CGameBoard::get_parking_occupation(int *tableau)
{
	for (int i = 0; i < HEIGHT * WIDTH; i++) {
		tableau[i] = 0;
	}

	for (unsigned int i = 0; i < this->vehicules.size(); i++) {
		// numerotation de la case reference.
		tableau[this->vehicules[i].position.m_y * 6 + this->vehicules[i].position.m_x] = 1+i;
		// numerotation de la case suivante en fonction de l'axe
		tableau[(this->vehicules[i].position.m_y + this->vehicules[i].axis)*6 + this->vehicules[i].position.m_x + 1 - this->vehicules[i].axis] = 1+i;
		// numerotation de la case suivante OU de la 2e case qui suit en fonction de la taille
		tableau[(this->vehicules[i].position.m_y + (this->vehicules[i].size-1)*this->vehicules[i].axis)*6+this->vehicules[i].position.m_x+(this->vehicules[i].size-1)*(1-this->vehicules[i].axis)] = 1+i;
	}
}

bool CGameBoard::is_move_impossible(int mvt, unsigned int num_vehicule)
{
	int cases_occ[HEIGHT * WIDTH];

	this->get_parking_occupation(cases_occ);

	int newAbs;
	int newOrd;
	int largeur = (this->vehicules[num_vehicule].size-1) * (1 - this->vehicules[num_vehicule].axis); // largeur
	int hauteur = (this->vehicules[num_vehicule].size-1) * this->vehicules[num_vehicule].axis; // hauteur

	if(mvt != -1 && mvt != 1) {
		return true;
	}

	if (mvt == 1) { // 1 : droite ou bas
		newAbs = this->vehicules[num_vehicule].position.m_x + (1 - this->vehicules[num_vehicule].axis); // horizontal
		newOrd = this->vehicules[num_vehicule].position.m_y + this->vehicules[num_vehicule].axis; // vertical
	} else { // -1 : gauche ou haut
		newAbs = this->vehicules[num_vehicule].position.m_x - (1 - this->vehicules[num_vehicule].axis); // horizontal
		newOrd = this->vehicules[num_vehicule].position.m_y - this->vehicules[num_vehicule].axis; // vertical
	}

	//Autorisation de sortie du parking
	if(newAbs+largeur > 5 && newOrd==2){
		return false;
	}

	// On vérifie si la voiture est entierement dans le damier
	if(newAbs < 0 || newOrd < 0 || newAbs+largeur > 5 || newOrd+hauteur > 5){
		return true;
	}

	// On vérifie si la case la plus en haut, ou la plus à gauche, est occupée
	if ( mvt == -1 && cases_occ[newOrd*6 + newAbs] != 0 ){
		return true;
	}

	// On vérifie si la case la plus en bas, ou la plus à droite, est occupée
	if( mvt == 1 && cases_occ[(newOrd+hauteur)*6 + (newAbs+largeur)] != 0 ){
		return true;
	}

	return false;
}

struct BoardAndPath
{
	CGameBoard config;
	t_chemin chemin;
};

t_chemin CGameBoard::solution(void)
{
	int nb_vehicules = this->vehicules.size();

	bool continuer = true;

	int nouveau = 0;

	int l = 1;

	t_chemin res;

	BinarySearchTree<CGameBoard> arbre;

	// Declaration de la file
	Queue<BoardAndPath> file;

	// Insertion du 1er maillon
	BoardAndPath BP;
	BP.chemin = (t_mouvement *)malloc(sizeof(t_mouvement));
	BP.chemin->voiture = -1;
	BP.chemin->deplacement = 0; //pour differencier l initialisation de la fin d un chemin.
	BP.config = *this;

	file.enqueue(BP);

	// Insertion de la configuration actuelle dans l'arbre
	arbre.add(*this);

	// Début de la resolution
	while (continuer)
	{
		for (int i = 0; i < nb_vehicules && continuer; i++)
		{
			for (int j = 0; j < 2 && continuer; j++)
			{
				const BoardAndPath & first = file.first();
				CGameBoard	CurrentConfig	= first.config;
				t_chemin	CurrentPath		= first.chemin;

				if (!CurrentConfig.is_move_impossible(2*j-1, i)) // mouvement possible
				{
					if (i == 0 && j == 1 && CurrentConfig.vehicules[0].position.m_x+1 == 5 && CurrentConfig.vehicules[0].position.m_y == 2 )
					{
						continuer = false; // victoire (fin condition sortie boucle while)
					}
					else
					{
						CurrentConfig.move(2*j-1, i); // mouvement
					}
				}

				nouveau = arbre.add(CurrentConfig); //ajout(&arbre, aux->suivant->config);

				// n'existe pas déjà dans l'arbre
				if (nouveau && continuer)
				{
					/* Compte la longueur du chemin */

					l = 0; // longueur reelle du chemin

					while (!(CurrentPath[l].voiture == -1))
					{
						++l;
					} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.


					/* Création d'un nouveau noeud */

					BoardAndPath BP;

					BP.config = CurrentConfig;

					BP.chemin = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); // longueur actuelle + ajout (1,-1)

					// Copie du chemin precedent
					for (int k = 0; k < l; k++)
					{
						BP.chemin[k].voiture = CurrentPath[k].voiture;
						BP.chemin[k].deplacement = CurrentPath[k].deplacement;
					}

					//ajout du nouveau deplacement
					BP.chemin[l].voiture = i;
					BP.chemin[l].deplacement = j;

					BP.chemin[l+1].voiture = -1;
					BP.chemin[l+1].deplacement = -1;

					file.enqueue(BP);
				}

				// Victoire
				if (!continuer)
				{
					l = 0; // longueur reelle du chemin

					while (!(CurrentPath[l].voiture == -1))
					{
						++l;
					} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.

					res = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); // longueur actuelle + ajout (1,-1)

					// Copie du chemin precedent
					for (int k = 0; k < l; k++)
					{
						res[k].voiture = CurrentPath[k].voiture;
						res[k].deplacement = CurrentPath[k].deplacement;
					}

					//ajout du nouveau deplacement
					res[l].voiture = i;
					res[l].deplacement = j;

					res[l+1].voiture = -1;
					res[l+1].deplacement = -1;
				}
			} // Fin boucle j
		} // Fin boucle i

		if (continuer)
		{
			file.dequeue();
		}

	} // Fin boucle continuer

	return res;
}
