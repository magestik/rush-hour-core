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
