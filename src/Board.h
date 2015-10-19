#pragma once

#include "Block.h"

namespace RushHour
{

template<unsigned int W, unsigned int H>
class Board
{

public:

	enum
	{
		MAX = ((W * H) / 2) - 1,
	};

	Board	(void);
	~Board	(void);

	bool add(const Block & block);

	void move(int mvt, unsigned int num_vehicule);

	void get_parking_occupation(int * tableau);

	bool is_move_impossible(int mvt, unsigned int num_vehicule);

	unsigned int getMaxNegativeMove(unsigned int index);
	unsigned int getMaxPositiveMove(unsigned int index);

	//t_chemin solution(void);

	Block & operator [] (unsigned int index)
	{
		return(m_aBlocks[index]);
	}

	const Block & operator [] (unsigned int index) const
	{
		return(m_aBlocks[index]);
	}

	unsigned int getBlockCount() const
	{
		return(m_iCount);
	}

public:

	Block m_aBlocks [MAX];
	unsigned int m_iCount;

};

}
