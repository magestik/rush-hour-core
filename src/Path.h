#pragma once

namespace RushHour
{

struct Move
{
	Move(void) : voiture(-1), deplacement(-1)
	{
		// ...
	}

	Move(short v, short d) : voiture(v), deplacement(d)
	{
		// ...
	}

	short voiture;
	short deplacement;
};


struct Path
{
	Path(void) : aMoves(nullptr), count(0)
	{
		// ...
	}

	Path(unsigned int count_) : aMoves(nullptr), count(0)
	{
		setCount(count_);
	}

	Path(const Path & p) : aMoves(nullptr), count(0)
	{

		unsigned int newCount = p.getCount();

		setCount(newCount);

		for (int i = 0; i < newCount; ++i)
		{
			setMove(i, p.getMove(i));
		}
	}

	~Path(void)
	{
		if (nullptr != aMoves)
		{
			delete [] aMoves;
			aMoves = nullptr;
		}
	}

	Path & operator = (const Path & p) = delete;

	const Move & getMove(unsigned int index) const
	{
		return(aMoves[index]);
	}

	void setMove(unsigned int index, const Move & m)
	{
		aMoves[index] = m;
	}

	void setCount(unsigned int count_)
	{
		if (nullptr == aMoves)
		{
			aMoves = new Move [count_];

			count = count_;
		}
	}

	unsigned int getCount(void) const
	{
		return(count);
	}

private:

	Move * aMoves;
	unsigned int count;
};

}
