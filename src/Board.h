#pragma once

#include "Block.h"
#include "Path.h"

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

	unsigned int getMaxNegativeMove(unsigned int index) const;
	unsigned int getMaxPositiveMove(unsigned int index) const;

	bool isCompleted(void) const;

	t_chemin solution(void) const;

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

protected:

	bool is_move_impossible(int mvt, unsigned int num_vehicule);

	void get_parking_occupation(int * tableau) const;

private:

	Block m_aBlocks [MAX];
	unsigned int m_iCount;

};


template<unsigned int W, unsigned int H>
inline int compare(const RushHour::Board<W,H> & p1, const RushHour::Board<W,H> & p2)
{
	const unsigned int count1 = p1.getBlockCount();
	const unsigned int count2 = p2.getBlockCount();
	//assert(count1 == count2);

	for (unsigned int i = 0; i < count1; ++i)
	{
		int diffX = p1[i].getX() - p2[i].getX();

		if (0 != diffX)
		{
			return diffX;
		}

		int diffY = p1[i].getY() - p2[i].getY();

		if (0 != diffY)
		{
			return diffY;
		}
	}

	return 0;
}
}
