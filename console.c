#include "console.h"

#include <stdio.h>
#include <stdlib.h>

#include "rush.h"
#include "resolution.h"

void console_init() {

	while ( console_jouer(*parking_actuel) ) {
		load_next_level();
	}
}

int console_resolution(t_parking parking) {
	t_chemin chemin;
	int k = 1;
	char pause;

	chemin = solution(parking);

	while( chemin[k].deplacement != -1 && chemin[k].voiture != -1 ){

		move(2*chemin[k].deplacement-1, chemin[k].voiture, parking);
		nb_coups++;

		console_affiche(parking);

		printf("\nVoiture n°%d, mouvement %d", chemin[k].voiture+1, chemin[k].deplacement);
		printf("\t(appuyez sur une touche pour continuer)");
		scanf("%c", &pause);

		k++;
	}

	nb_coups++;
	console_affiche(parking);

	printf("\tRésolution en %d mouvements", chemin[k].deplacement);
	printf("\t(appuyez sur une touche pour continuer)");
	scanf("%c", &pause);

	return 5;
}

int console_jouer(t_parking parking) {
	int reponse = -1;
	int num_vehicule = 0;

	while ( reponse ) {
		console_affiche(parking);
		reponse = console_menu(2);

		switch( reponse ) {

			case 17:
				console_resolution(parking);
				return 1;
			break;

			case 18:
				return 1;
	  		break;

			case 0:
				return 0;
			break;

			default:
				num_vehicule = reponse-1;
				while(num_vehicule >= parking.nb_vehicules || (is_move_possible(1, num_vehicule, parking) && is_move_possible(2, num_vehicule, parking)) ) {
					printf("\nNumero de vehicule non valide ou vehicule bloqué\n");
					scanf("%d", &reponse);
					scanf("%*[^\ns]");
					getchar();
					num_vehicule = reponse-1;
				}
		}

		console_affiche(parking);
		reponse = console_menu(3);
		while ( is_move_possible(reponse, num_vehicule, parking) ) {
		    console_affiche(parking);
		    printf("\t\t(\033[1;31m mouvement impossible \033[0m) \n");
		    reponse = console_menu(3);
		}

		move(2*(reponse-1)-1, num_vehicule, parking);
		nb_coups++;

		// condition de victoire
		if ( parking.position[0].abs + 1 > 5 && parking.position[0].ord == 2 ) { // condition sur abscisse suffisante, non ?
			return 1;
		}
	}

	return 0;
}



int coups(t_parking parking){
	int compteur = 0;

	t_chemin chemin = solution(parking);

	while (chemin->voiture != -1) {
		compteur++;
		chemin++;
	}

	return compteur;
}

/************************  Affichage des Menus  *****************************/

int console_menu(int choix_menu) {
	int reponse;

	printf("\n\n******  MENU  ******\n");

	switch (choix_menu) {
    	case 2:
    		printf("- indiquez le numéro du vehicule à déplacer\n");
    		printf("- pour visualiser une solution, tapez 17\n");
    		printf("- pour démarrer une autre grille, tapez 18\n");
			printf("- pour sortir du jeu, tapez 0\n");
   		break;

		case 3:
			printf("Quel deplacement souhaitez-vous effectuer ?\n");
		break;
	}

	scanf("%d", &reponse);
	scanf("%*[^\ns]");
	getchar();

	return reponse;
}

/**************************  Affichage de le parking ****************************/

void console_affiche(t_parking parking) {
	int i, j, n;

	int parkingGUI[HEIGHT * WIDTH];
	get_parking_occupation(parking, parkingGUI);

	printf("\033[H\033[2J"); //clear
	printf("\t\t    __________________ \n");
	printf("\t\t   |                  |\n");

	for (j=0; j<HEIGHT; j++){
		printf("\t\t   |");

		for (i=0; i<WIDTH; i++){
			n = *(parkingGUI + 6 * j + i);
			if( n == 0) {
				printf(" . ");
			} else {
				if ( n < 10 ) {
					if ( n != 9 ){
						printf("\033[7;3%dm %d \033[0m", n%8, n);
					} else {
						printf("\033[7;3%dm %d \033[0m", 2, n);
					}

				} else {
					printf("\033[7;3%dm %d\033[0m", (n+1)%8, n);
				}
	  		}
	  	}

	  	if(j == 2){
	  		printf(" \n");
	  	} else {
	  		printf("|\n");
	  	}
    }

    printf("\t\t   |__________________|\n");

    printf("\n\n\n\t\t Nombre de coups joués : %d \n", nb_coups);
}

/**************************  Affichage victoire  *******************************/

void gagner() {
	printf("\033[H\033[2J"); // clear
    printf("\n\n\n\t\t  c'est GAGNE !!!\n\n\n");
    printf("\t\t\t\t c'est GAGNE !!!\n\n\n");
    printf("\t\t\t\t\t\t c'est GAGNE !!!\n\n\n");
    printf("\t\t\t      ______________\n");
	printf("\t\t\t     //    | |      \\      \n");
	printf("\t\t\t  __//_____| |_______\\_____\n");
	printf("\t\t\t |  _       | -     _     C|\n");
	printf("\t\t\t=|_/ \\______|______/ \\_____|\n");
	printf("\t\t\t   \\_/             \\_/\n");
	printf("\n");
    printf("\n\n\t\t\tVous avez gagné en %d coups !\n\n\n", nb_coups);
	//exit(0);
}
