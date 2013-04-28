#include "pre.h"

/*
 * Enleve le premier element de la file
 */
void defiler(t_file *pF) {
	t_file aux = *pF;
	*pF = (*pF)->suivant;
	free(aux->chemin);
	delete aux;
}

void free_file(t_file file){
    if (file != NULL){
        while (file->suivant != NULL){
            defiler(&file);
        }
    }
}
