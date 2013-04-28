#include "pre.h"

static FILE *level_file;

#define FILE_SIZE_VEHICULE 8

void init_level_file(std::string filename) {
	level_file = fopen(filename.c_str(), "r");

	if (NULL == level_file) {
		perror("Couldn't open level file");
		exit(1);
	}

	parking_actuel = read_level_file();
}

void close_level_file() {
	fclose(level_file);
}

CGameBoard *parse_level_line(char *line, int len) {
	char buf;
	t_position pos;
	int size;
	// FIXME: free if return NULL
	CGameBoard *liste = new CGameBoard();


	if (0 != len % 8 ) {
		std::cout << "LEN != 8 => " << len << std::endl;
		return NULL;
	}

	// X;Y;S;A;

	for (int i = 0; i < len/8; i++) {

		int j = i * 8;

		// Parse X
		buf = line[j];

		if( buf < '0' || buf > '5' ) {
			std::cout << "X" << std::endl;
			return NULL;
		}

		pos.abs = buf - '0';

		// Parse Y
		buf = line[j + 2];

		if( buf < '0' || buf > '5' ) {
			std::cout << "Y" << std::endl;
			return NULL;
		}

		pos.ord = buf - '0';

		// Parse Size
		buf = line[j + 4];

		if( buf < '2' || buf > '3' ) {
			std::cout << "size" << std::endl;
			return NULL;
		}

		size = buf - '0';

		// Parse Axis
		buf = line[j + 6];

		if( buf == '0' ) {
			liste->vehicules.push_back(new CGameBlock(pos, size, CGameBlock::e_axis_vertical));
		} else if ( buf == '1') {
			liste->vehicules.push_back(new CGameBlock(pos, size, CGameBlock::e_axis_horizontal));
		} else {
			std::cout << "axis" << std::endl;
			return NULL;
		}


	}


	return liste;
}

CGameBoard *read_level_file() {

	int len;
	char buf[FILE_SIZE_VEHICULE * MAX_VEHICULE];

	CGameBoard *liste = NULL;

	// on cherche la prochaine ligne contenant une configuration
	while ( !feof(level_file) && liste == NULL ) {

		fgets(buf, FILE_SIZE_VEHICULE * MAX_VEHICULE, level_file); // on lit un ligne

		len = std::string(buf).length() - 1;

		if( buf[0] != '#' ){
			liste = parse_level_line(buf, len);

			if( liste == NULL ) {
				std::cerr << "Bad level line !" << std::endl;
			}
		}
	}

	if( feof(level_file) && liste == NULL ) {
		std::cout << "Congratulations !" << std::endl;
		exit(0); // FIXME
	}

	return liste;
}

/*
 * Charge le prochain niveau du fichier
 */
void load_next_level() {
	//free(parking_actuel->vehicule);
	//free(parking_actuel->position);
	free(parking_actuel);

	parking_actuel = read_level_file();
}

