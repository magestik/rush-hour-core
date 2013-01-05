#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "level.h"

FILE *level_file;

void init_level_file(char *filename) {
	level_file = fopen(filename, "r");
	if( level_file == NULL ){
		perror("Couldn't open level file");
		exit(1);
	}

	parking_actuel = read_level_file();
}

void close_level_file(){
	fclose(level_file);
}

t_parking *parse_level_line(char *line, int len) {
	int i, j;
	char buf;

	// FIXME: free if return NULL
	t_parking *liste = malloc(sizeof(t_parking));
	t_vehicule *liste_vehicule = malloc(sizeof(t_vehicule) * MAX_VEHICULE);
	t_position *liste_position = malloc(sizeof(t_position) * MAX_VEHICULE);


	if( len % 8 != 0 ){
		return NULL;
	}

	liste->nb_vehicules = len / 8;

	// X;Y;S;A;

	for (i = 0; i < liste->nb_vehicules; i++) {

		j = i * 8;

		// Parse X
		buf = line[j];

		if( buf < '0' || buf > '5' ) {
			return NULL;
		}

		liste_position[i].abs = buf - '0';

		// Parse Y
		buf = line[j + 2];

		if( buf < '0' || buf > '5' ) {
			return NULL;
		}

		liste_position[i].ord = buf - '0';

		// Parse Size
		buf = line[j + 4];

		if( buf < '2' || buf > '3' ) {
			return NULL;
		}

		liste_vehicule[i].taille = buf - '0';

		// Parse Axis
		buf = line[j + 6];

		if( buf != '0' && buf != '1' ) {
			return NULL;
		}

		liste_vehicule[i].axe = buf - '0';
	}

	liste->position = liste_position;
	liste->vehicule = liste_vehicule;

	return liste;
}

t_parking *read_level_file() {

	int len;
	char buf[FILE_SIZE_VEHICULE * MAX_VEHICULE];

	t_parking *liste = NULL;

	// on cherche la prochaine ligne contenant une configuration
	while ( !feof(level_file) && liste == NULL ) {
		fgets(buf, FILE_SIZE_VEHICULE * MAX_VEHICULE, level_file); // on lit un ligne

		len = strlen(buf) - 1;

		if( buf[0] != '#' ){
			liste = parse_level_line(buf, len);

			if( liste == NULL ) {
				fprintf(stderr, "Bad level line !\n");
			}
		}
	}

	if( feof(level_file) && liste == NULL ) {
		printf("\n\nCongratulations !\n\n");
		exit(0); // FIXME
	}

	return liste;
}
