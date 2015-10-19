#include "pre.h"

#include <string.h>

#define FILE_SIZE_VEHICULE 8

/**
 * Constructor
 */
LevelManager::LevelManager(void)
{

}

/**
 * Destructor
 */
LevelManager::~LevelManager(void)
{

}

/**
 * Initializer
 */
bool LevelManager::Initialize(const char * filename)
{
	m_pFile = fopen(filename, "r");

	return(NULL != m_pFile);
}

/**
 * Release file
 */
void LevelManager::Release(void)
{
	fclose(m_pFile);
}

/**
 * Get Next Level
 */
CGameBoard LevelManager::getNextLevel(void)
{
	char buf[FILE_SIZE_VEHICULE * MAX_VEHICULE];

	CGameBoard liste;

	// on cherche la prochaine ligne contenant une configuration
	while (!feof(m_pFile))
	{
		fgets(buf, FILE_SIZE_VEHICULE * MAX_VEHICULE, m_pFile); // on lit un ligne



		if (buf[0] != '#')
		{
			liste = parse_level_line(buf);
			break;
/*
			if( liste == NULL ) {
				std::cerr << "Bad level line !" << std::endl;
			}
*/
		}
	}

	if (feof(m_pFile))
	{
		printf("Congratulations !\n");
		exit(0); // FIXME
	}

	return liste;
}

/**
 * Parse one Level (one line in the file)
 */
CGameBoard LevelManager::parse_level_line(char * line)
{
	char buf;
	t_position pos;
	int size;

	int len = strlen(line) - 1;

	CGameBoard liste;

	if (0 != len % FILE_SIZE_VEHICULE )
	{
		printf("LEN != 8 => %d\n", len);
		return liste;
	}

	// X;Y;S;A;

	for (int i = 0; i < len/FILE_SIZE_VEHICULE; i++)
	{

		int j = i * FILE_SIZE_VEHICULE;

		// Parse X
		buf = line[j];

		if( buf < '0' || buf > '5' )
		{
			printf("X\n");
			return liste;
		}

		pos.m_x = buf - '0';

		// Parse Y
		buf = line[j + 2];

		if( buf < '0' || buf > '5' )
		{
			printf("Y\n");
			return liste;
		}

		pos.m_y = buf - '0';

		// Parse Size
		buf = line[j + 4];

		if( buf < '2' || buf > '3' )
		{
			printf("size\n");
			return liste;
		}

		size = buf - '0';

		// Parse Axis
		buf = line[j + 6];

		if( buf == '0' )
		{
			liste.vehicules.push_back(CGameBlock(pos, size, CGameBlock::e_axis_vertical));
		}
		else if ( buf == '1')
		{
			liste.vehicules.push_back(CGameBlock(pos, size, CGameBlock::e_axis_horizontal));
		}
		else
		{
			printf("axis\n");
			return liste;
		}
	}

	return liste;
}
