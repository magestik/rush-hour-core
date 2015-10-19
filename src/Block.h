#pragma once

namespace RushHour
{

struct Block
{

public:

	enum EAxis
	{
		e_axis_horizontal = 0,
		e_axis_vertical = 1,
	};

	Block		(void);
	Block		(unsigned int posX, unsigned int posY, unsigned int size, EAxis axis);
	~Block		(void);

	EAxis getAxis(void) const
	{
		return((EAxis)(bits & 1));
	}

	unsigned int getSize(void) const
	{
		return(((bits >> 1) & 0x1) + 2); // 0 => 2, 1 => 3
	}

	unsigned int getX(void) const
	{
		return((bits >> 2) & 0x7); // 7 = 0b111
	}

	unsigned int getY(void) const
	{
		return((bits >> 5) & 0x7); // 7 = 0b111
	}

	void setX(unsigned int pos)
	{
		bits = (bits & 0xE3) | ((pos & 0x7) << 2); // 0xE3 = 0b11100011
	}

	void setY(unsigned int pos)
	{
		bits = (bits & 0x1F) | ((pos & 0x7) << 5); // 0x1F = 0b00011111
	}

private:

	// bit 0   : axis
	// bit 1   : size - 2
	// bit 2-4 : position x
	// bit 5-7 : position y
	unsigned char bits;

};

}
