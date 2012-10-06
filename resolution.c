#include "resolution.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * Teste si une configuration de parking est 'strictement superieure' a une autre configuration
 * selon l ordre defini
 */
int superieur(t_parking p1,t_parking p2){
	int nb_vehicules = p1.nb_vehicules;
	int i=0;
	while (i<nb_vehicules){
		if (p1.position[i].abs>p2.position[i].abs)
			return 1;
		else {
			if(p1.position[i].abs==p2.position[i].abs) { 
			 	if (p1.position[i].ord>p2.position[i].ord) return 1;
			 	else {
			 		if (p1.position[i].ord==p2.position[i].ord) i++;
			 		else return 0;
			 	}
			}
			else return 0;
		}
	}
	return 0;
}



/*
 * Teste si une configuration de parking est 'egale' a une autre configuration
 * selon l ordre defini
 */
int egal(t_parking p1, t_parking p2){
	int nb_vehicules = p1.nb_vehicules;
	int i;
	for (i=0;i<nb_vehicules;i++){
		if (p1.position[i].abs!=p2.position[i].abs || p1.position[i].ord!=p2.position[i].ord)
			return 0;
	}
	return 1;
}




/*
 * Ajoute une configuration dans un ABR s'il n'est pas deja present
 * Renvoi 1 si l'ajout est effectué
 * Renvoi 0 si l'ajout n'est pas effectué
 */
int ajout(t_ABR *pa, t_parking p){
	if (*pa==NULL) {
		int k;
		*pa=malloc(sizeof(t_Noeud));
		(*pa)->config.nb_vehicules=p.nb_vehicules;
		(*pa)->config.vehicule = malloc(sizeof(t_vehicule)*p.nb_vehicules);
		(*pa)->config.position = malloc(sizeof(t_position)*p.nb_vehicules);
		for (k=0;k<p.nb_vehicules;k++){
					(*pa)->config.vehicule[k].axe=p.vehicule[k].axe;
					(*pa)->config.vehicule[k].taille=p.vehicule[k].taille;
					(*pa)->config.position[k].abs=p.position[k].abs;
					(*pa)->config.position[k].ord=p.position[k].ord;
		}
		(*pa)->f_gauche=NULL;
		(*pa)->f_droit=NULL;
		return 1;
	}
	else {
		if (egal(p,(*pa)->config)) {
			return 0;
		}
		else {
			if (superieur(p,(*pa)->config)){
				return ajout(&((*pa)->f_droit), p);
			}
			else {
				return ajout(&((*pa)->f_gauche), p);
			}
		}
	}
}



/*
 * Enleve le premier element de la file
 */
void defiler(t_file *pF){
	t_file aux=*pF;
	*pF=(*pF)->suivant;
	free(aux->config.vehicule);
	free(aux->config.position); 
	free(aux);
}

void free_arbre(t_ABR arbre){
    t_ABR fg, fd;
    if (arbre != NULL){
        fg = arbre->f_gauche;
        fd = arbre->f_droit;
        free(arbre);
        arbre=NULL;
        if(fg!=NULL) free_arbre(fg);
        if(fd!=NULL) free_arbre(fd);
    }
}

void free_file(t_file file){
    if (file != NULL){
        while (file->suivant != NULL){
            defiler(&file);
        }
    }
}


/**************************  Fonction de resolution automatique ***************/		

/*
 * Resoud le jeu à partir de la configuration PARKING
 * Renvoie le chemin entre la configuration initiale et la configuration finale
 */
t_chemin solution(t_parking  parking){
	int continuer,i,j,k,nouveau;
	int nb_vehicules=parking.nb_vehicules;
	
	//declaration de la file et insertion du 1er parking
	t_file file, aux;
	file=malloc(sizeof(t_Maillon));
	file->config.nb_vehicules=parking.nb_vehicules;
	file->config.vehicule = malloc(sizeof(t_vehicule)*parking.nb_vehicules);
	file->config.position = malloc(sizeof(t_position)*parking.nb_vehicules);
	for (k=0;k<nb_vehicules;k++){
		file->config.vehicule[k].axe=parking.vehicule[k].axe;
		file->config.vehicule[k].taille=parking.vehicule[k].taille;
		file->config.position[k].abs=parking.position[k].abs;
		file->config.position[k].ord=parking.position[k].ord;
	}
	file->suivant=NULL;
	file->chemin=malloc(sizeof(t_mouvement));
	
	file->chemin->voiture=-1;
	file->chemin->deplacement=0;//pour differencier l initialisation de la fin d un chemin.
	aux=file;
	int l=1;
	t_ABR arbre = NULL;
	ajout(&arbre, parking);	
	continuer=1;
	
	//Début de la resolution
	
	while (continuer){
	
		for (i=0;i<nb_vehicules;i++){				
		
			for(j=0;j<2;j++){
	
				if (continuer){
					aux->suivant=malloc(sizeof(t_Maillon));
					aux->suivant->config.nb_vehicules=nb_vehicules;
					aux->suivant->config.vehicule = malloc(sizeof(t_vehicule)*nb_vehicules);
					aux->suivant->config.position = malloc(sizeof(t_position)*nb_vehicules);
				
					for (k=0;k<nb_vehicules;k++){
						aux->suivant->config.vehicule[k].axe=file->config.vehicule[k].axe;
						aux->suivant->config.vehicule[k].taille=file->config.vehicule[k].taille;
						aux->suivant->config.position[k].abs=file->config.position[k].abs;
						aux->suivant->config.position[k].ord=file->config.position[k].ord;
					}
				
					if (!mvt_impossible(j+1,i,aux->suivant->config)){
					/* remarque perso 1/2 : 
					 * ici on utilise "j+1" car j varie entre 0 et 1
					 * et MVT_IMPOSSIBLE prend un mouvement = 1 ou 2
					 */ 
						if(i==0 && j==1 && aux->suivant->config.position[0].abs+1 == 5 && aux->suivant->config.position[0].ord==2){
							continuer=0;
						} 							//soit victoire (fin condition sortie boucle while)
						else bouger(2*j-1,i,aux->suivant->config);		//soit bouger
						/* remarque perso 2/2 : 
						 * ici on utilise "2j-1" car j varie entre 0 et 1
						 * et BOUGER prend un mouvement = -1 ou 1
						 */ 
					}
								
					nouveau=ajout(&arbre, aux->suivant->config);

					if (nouveau ||continuer==0) {

						l=0; //longueur reelle du chemin
						while (!(file->chemin[l].voiture==-1)){
							l++;
						}//RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.
			
						aux->suivant->chemin=malloc(sizeof(t_mouvement)*(l+2)); //longueur relle + ajout +  (-1,-1)
						//copie du chemin precedent
						for(k=0;k<l;k++){
							aux->suivant->chemin[k].voiture=file->chemin[k].voiture;
							aux->suivant->chemin[k].deplacement=file->chemin[k].deplacement;
						}
						//ajout du nouveau deplacement
						aux->suivant->chemin[l].voiture=i;
						aux->suivant->chemin[l].deplacement=j;
				
						aux->suivant->chemin[l+1].voiture=-1;
						aux->suivant->chemin[l+1].deplacement=-1;
				
						aux=aux->suivant;
						aux->suivant=NULL;

				
					}
		
					else{
						 free(aux->suivant);
						 aux->suivant=NULL;
					}
					
				
				}// Fin if continuer
		
			}											// Fin boucle j
		
		}												// Fin boucle i
		if (continuer) defiler(&file);
														
	}					// Fin boucle continuer
	free_arbre(arbre);
	free_file(file);
	return aux->chemin;												
}
