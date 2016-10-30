#include "Block.h"

#include <assert.h>

/**
 * @brief Default constructor
 */
RushHour::Block::Block(void) : bits(0)
{
	// ...
}

/**
 * @brief RushHour::Block::Block
 * @param posX
 * @param posY
 * @param size
 * @param axis
 */
RushHour::Block::Block(unsigned int posX, unsigned int posY, unsigned int size, EAxis axis) : bits(0)
{
	assert(2 == size || 3 == size);
	assert(0 == axis || 1 == axis);
	assert(posX <= 5); // posX is unsigned, we don't have to check > 0
	assert(posY <= 5); // posY is unsigned, we don't have to check > 0

	bits = ((posY & 7) << 5) | ((posX & 7) << 2) | ((size - 2) << 1) | axis;
}

/**
 * @brief Destructor
 */
RushHour::Block::~Block(void)
{
	// ...
}

/**
 * @brief RushHour::Block::intersects
 * @param block
 * @return
 */
bool RushHour::Block::intersects(const Block & block) const
{
	if (block.getAxis() == getAxis())
	{
		if (e_axis_horizontal == block.getAxis())
		{
			if (block.getY() != getY())
			{
				return(false);
			}

			if (block.getX() < getX())
			{
				return((block.getX() + block.getSize() - 1) >= getX());
			}
			else
			{
				return((getX() + getSize() - 1) >= block.getX());
			}
		}
		else // e_axis_vertical == block.getAxis()
		{
			if (block.getX() != getX())
			{
				return(false);
			}

			if (block.getY() < getY())
			{
				return((block.getY() + block.getSize() - 1) >= getY());
			}
			else
			{
				return((getY() + getSize() - 1) >= block.getY());
			}
		}
	}
	else
	{
		if (e_axis_horizontal == block.getAxis())
		{
			unsigned int X1 = block.getX();
			unsigned int X2 = block.getX() + block.getSize() -1;
			unsigned int Y1 = this->getY();
			unsigned int Y2 = this->getY() + this->getSize() - 1;
			return(X1 <= this->getX() && X2 >= this->getX() && Y1 <= block.getY() && Y2 >= block.getY());
		}
		else
		{
			unsigned int X1 = this->getX(); // 2
			unsigned int X2 = this->getX() + this->getSize() - 1; // 3
			unsigned int Y1 = block.getY(); // 0
			unsigned int Y2 = block.getY() + block.getSize() - 1; // 1
			return(X1 <= block.getX() && X2 >= block.getX() && Y1 <= this->getY() && Y2 >= this->getY());
		}
	}

	return(false); // unreachable
}
