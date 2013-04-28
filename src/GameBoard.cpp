#include "pre.h"

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

t_chemin CGameBoard::solution(void) {
	int nb_vehicules = this->vehicules.size();

	bool continuer = true;

	int nouveau = 0;

	int l = 1;

	BinarySearchTree arbre;

	//declaration de la file et insertion du 1er maillon
	t_Maillon * file = new t_Maillon;

	file->suivant = NULL;
	file->chemin = (t_mouvement *)malloc(sizeof(t_mouvement));
	file->chemin->voiture = -1;
	file->chemin->deplacement = 0; //pour differencier l initialisation de la fin d un chemin.
	file->config = *this;

	t_file aux = file;

	arbre.add(*this);

	// Début de la resolution

	while (continuer) {
		for (int i = 0; i < nb_vehicules && continuer; i++) {
			for (int j = 0; j < 2 && continuer; j++) {

				aux->suivant = new t_Maillon;
				aux->suivant->config = file->config;

				if (!aux->suivant->config.is_move_impossible(2*j-1, i)) {

					if (i == 0 && j == 1 && aux->suivant->config.vehicules[0].position.m_x+1 == 5 && aux->suivant->config.vehicules[0].position.m_y == 2 ){
						continuer = false; // victoire (fin condition sortie boucle while)
					} else {
						aux->suivant->config.move(2*j-1, i); // mouvement
					}
				}

				nouveau = arbre.add(aux->suivant->config); //ajout(&arbre, aux->suivant->config);

				if (nouveau || continuer == false) {
					l = 0; // longueur reelle du chemin

					while (!(file->chemin[l].voiture==-1)){
						l++;
					} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.

					aux->suivant->chemin = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); //longueur relle + ajout (1,-1)

					//copie du chemin precedent
					for(int k = 0; k < l; k++) {
						aux->suivant->chemin[k].voiture = file->chemin[k].voiture;
						aux->suivant->chemin[k].deplacement = file->chemin[k].deplacement;
					}

					//ajout du nouveau deplacement
					aux->suivant->chemin[l].voiture = i;
					aux->suivant->chemin[l].deplacement = j;

					aux->suivant->chemin[l+1].voiture = -1;
					aux->suivant->chemin[l+1].deplacement = -1;

					aux = aux->suivant;
					aux->suivant = NULL;
				} else {
					//free(aux->suivant->chemin);
					delete aux->suivant;
					aux->suivant = NULL;
				}
			} // Fin boucle j
		} // Fin boucle i

		if (continuer) {
			defiler(&file);
		}

	} // Fin boucle continuer

	free_file(file);

	return aux->chemin;
}
