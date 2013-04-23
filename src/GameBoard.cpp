#include "pre.h"

/*explicit*/ CGameBoard::CGameBoard(void)
: vehicules()
{
	this->vehicules.reserve(MAX_VEHICULE);
}

/*virtual*/ CGameBoard::~CGameBoard(void)
{

}

t_chemin CGameBoard::solution(void) {
	int nb_vehicules = this->vehicules.size();

	bool continuer = true;

	int nouveau = 0;

	int l = 1;

	t_ABR arbre = NULL;

	//declaration de la file et insertion du 1er maillon
	t_Maillon * file = new t_Maillon;

	file->suivant = NULL;
	file->chemin = (t_mouvement *)malloc(sizeof(t_mouvement));
	file->chemin->voiture = -1;
	file->chemin->deplacement = 0; //pour differencier l initialisation de la fin d un chemin.

	for (int k = 0; k < nb_vehicules; k++){
		file->config.vehicules.push_back(new CGameBlock(this->vehicules[k]->position, this->vehicules[k]->size, this->vehicules[k]->axis));
	}

	t_file aux = file;

	ajout(&arbre, *this);

	// Début de la resolution

	while (continuer) {
		for (int i = 0; i < nb_vehicules; i++) {
			for (int j = 0; j < 2; j++) {

				if (continuer) {
					aux->suivant = new t_Maillon;

					for (int k = 0; k < nb_vehicules; k++) {
						aux->suivant->config.vehicules.push_back(new CGameBlock(file->config.vehicules[k]->position, file->config.vehicules[k]->size, file->config.vehicules[k]->axis));
					}

					if (!is_move_possible(j+1, i, aux->suivant->config)) {
						/* remarque perso 1/2 :
						 * ici on utilise "j+1" car j varie entre 0 et 1
						 * et MVT_IMPOSSIBLE prend un mouvement = 1 ou 2
						 */

						if( i == 0 && j == 1 && aux->suivant->config.vehicules[0]->position.abs+1 == 5 && aux->suivant->config.vehicules[0]->position.ord == 2 ){
							continuer = false; // soit victoire (fin condition sortie boucle while)
						} else {
							move(2*j-1, i, aux->suivant->config); // soit bouger
						}

						/* remarque perso 2/2 :
						 * ici on utilise "2j-1" car j varie entre 0 et 1
						 * et BOUGER prend un mouvement = -1 ou 1
						 */
					}

					nouveau = ajout(&arbre, aux->suivant->config);

					if (nouveau || continuer == false) {
						l = 0; // longueur reelle du chemin

						while (!(file->chemin[l].voiture==-1)){
							l++;
						} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.

						aux->suivant->chemin = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); //longueur relle + ajout +  (-1,-1)

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
						free(aux->suivant);
						aux->suivant = NULL;
					}
				} // Fin if continuer
			} // Fin boucle j
		} // Fin boucle i

		if (continuer) {
			defiler(&file);
		}

	} // Fin boucle continuer

	free_arbre(arbre);
	free_file(file);

	return aux->chemin;
}
