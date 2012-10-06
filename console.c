#include "console.h"

#include <stdio.h>
#include <stdlib.h>

#include "rush.h"
#include "resolution.h"

void console_init() {
	int menu = 1;
	int reponse = -1;

	console_affiche(*parking_actuel);

	reponse = console_menu(menu);

	while ( reponse ) {

		switch ( reponse ) {
			case 1:
				menu = console_jouer(*parking_actuel);
			break;

			case 2:
				menu = console_resolution(*parking_actuel);
			break;

			case 3:
				load_next_level();
				menu = 1;
			break;
		}

		if( menu ){
			if ( menu == 4 ) {
				//parking_actuel = configuration_load(niveau);
				load_next_level();
			}
			console_affiche(*parking_actuel);
			reponse = console_menu(menu);
		} else {
			reponse = 0;
		}
	}
}

int console_resolution(t_parking parking) {
	t_chemin chemin;
	int k, c;
	char pause;

	printf("\nCombien de coups souhaitez-vous voir ?\n");
	scanf("%d", &c);
	scanf("%*[^\ns]");
	getchar();

	chemin = solution(parking);

	for (k = 0; k < c; k++) {
		//ici je teste les 2 (== -1) pour ne pas tomber sur l'initialisation.
		if (chemin[k+1].deplacement==-1 && chemin[k+1].voiture==-1) {
			return 4;
		} else {
			move(2*chemin[k].deplacement-1, chemin[k].voiture, parking);
			nb_coups++;

			console_affiche(parking);

			printf("\nVoiture n°%d, mouvement %d",chemin[k].voiture+1,chemin[k].deplacement);
			printf("\t(appuyez sur une touche pour continuer)");

			scanf("%c", &pause);
		}
	}

	return 5;
}

int console_jouer(t_parking parking) {
	int reponse = -1;
	int num_vehicule = 0;

	while ( reponse ) {
		console_affiche(parking);
		reponse = console_menu(2);

		switch( reponse ) {
			case 17: //resolution auto
				if (console_resolution(parking)) {
					return 4;
				} else {
					return 5;
				}
			break;

			case 18:
				return 1;
	  		break;

			case 19:
				load_next_level(); //autre grille
				return 1;
	  		break;

			case 0:
				return 0;
			break;

			default:
				num_vehicule = reponse-1;
				while(num_vehicule >= parking.nb_vehicules || (is_move_possible(1, num_vehicule, parking) && is_move_possible(2, num_vehicule, parking)) ) {
					printf("\nNumero de vehicule non valide \n");
					scanf("%d",&reponse);
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
		if (parking.position[0].abs+1 > 5 && parking.position[0].ord == 2) { //condition sur abscisse suffisante, non ?
			return 4;
		}
	}

	return 0;
}



int coups(t_parking parking){
	int compteur = 0;
	t_chemin chemin = solution(parking);
	while (chemin->voiture!=-1) {
		compteur++;
		chemin=chemin+1;
	}
	return compteur;
}
/************************  Affichage des Menus  *****************************/

int console_menu(int choix_menu) {
	int reponse;

	printf("\n\n******  MENU  ******\n");

	switch (choix_menu) {
		case 1:
    		printf("- pour commencer la partie, tapez 1");
    		printf("  (nombre  de coups minimums : %d)\n", coups(*parking_actuel));
    		printf("- pour visualiser une solution pour cette partie, tapez 2\n");
    		printf("- pour charger une autre grille, tapez 3\n");
    		printf("- pour sortir du jeu, tapez 0\n");
		break;

    	case 2:
    		printf("- indiquez le numéro du vehicule à déplacer\n");
    		printf("- pour visualiser une solution, tapez 17\n");
			printf("- pour recommencer cette partie, tapez 18\n");
    		printf("- pour démarrer une autre grille, tapez 19\n");
			printf("- pour sortir du jeu, tapez 0\n");
   		break;

		case 3:
			printf("Quel deplacement souhaitez-vous effectuer ?\n");
		break;

		case 4:
			nb_coups++;
			gagner();
			nb_coups=0;
			printf("- pour recommencer la partie, tapez 1\n");
			printf("- pour visualiser une solution pour cette partie, tapez 2\n");
			printf("- pour charger une autre grille, tapez 3\n");
			printf("- pour sortir du jeu, tapez 0\n");
		break;

		case 5:
			printf("- pour continuer la partie, tapez 1\n");
			printf("- pour visualiser la suite de la solution pour cette partie, tapez 2\n");
			printf("- pour charger une autre grille, tapez 3\n");
			printf("- pour sortir du jeu, tapez 0\n");
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
	int * parkingGUI = tab_positions(parking);

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
