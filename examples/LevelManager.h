#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>

class LevelManager
{

public:

	explicit	LevelManager	(void);
	virtual		~LevelManager	(void);

	bool		Initialize		(const char * filename);
	void		Release			(void);

	CGameBoard	getNextLevel	(void);

private:

	CGameBoard parse_level_line(char * line);

	FILE * m_pFile;

};

#endif
