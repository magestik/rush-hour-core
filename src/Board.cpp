#include "Board.h"

#include <stdlib.h>
#include <assert.h>

#include <BinarySearchTree.h>
#include <Queue.h>

struct BoardAndPath
{
	BoardAndPath(void) : board(), path()
	{

	}

	BoardAndPath(const RushHour::Board<6,6> & newBoard, const RushHour::Path & newPath) : board(newBoard), path(newPath)
	{

	}

	RushHour::Board<6,6> board;
	RushHour::Path path;
};

struct BoardAndDistance
{
	RushHour::Board<6,6> board;
	unsigned int distance;
};

/**
 * @brief Constructor
 */
template<unsigned int W, unsigned int H>
RushHour::Board<W,H>::Board(void) : m_iCount(0)
{
	// ...
}

/**
 * @brief Destructor
 */
template<unsigned int W, unsigned int H>
RushHour::Board<W,H>::~Board(void)
{
	// ...
}

/**
 * @brief Add a block in the board
 */
template<unsigned int W, unsigned int H>
bool RushHour::Board<W,H>::add(const Block & block)
{
	assert(m_iCount < MAX);

	//
	// check if we can add the block
	for (int i = 0; i < m_iCount; ++i)
	{
		if (m_aBlocks[i].intersects(block))
		{
			return(false);
		}
	}

	// add the block
	m_aBlocks[m_iCount] = block;
	++m_iCount;

	return(true);
}

/**
 * @brief Move a block
 */
template<unsigned int W, unsigned int H>
void RushHour::Board<W,H>::move(int mvt, unsigned int index)
{
	if (index < m_iCount)
	{
		m_aBlocks[index].setX(m_aBlocks[index].getX() + (1 - int(m_aBlocks[index].getAxis())) * mvt);
		m_aBlocks[index].setY(m_aBlocks[index].getY() + int(m_aBlocks[index].getAxis()) * mvt);
	}
}

/**
 * @brief Return a matrix representing board occupation
 */
template<unsigned int W, unsigned int H>
void RushHour::Board<W,H>::get_parking_occupation(int * tableau) const
{
	for (int i = 0; i < W * H; i++)
	{
		tableau[i] = 0;
	}

	for (unsigned int i = 0; i < m_iCount; i++)
	{
		// numerotation de la case reference.
		tableau[m_aBlocks[i].getY() * 6 + m_aBlocks[i].getX()] = 1+i;
		// numerotation de la case suivante en fonction de l'axe
		tableau[(m_aBlocks[i].getY() + int(m_aBlocks[i].getAxis()))*6 + m_aBlocks[i].getX() + 1 - int(m_aBlocks[i].getAxis())] = 1+i;
		// numerotation de la case suivante OU de la 2e case qui suit en fonction de la taille
		tableau[(m_aBlocks[i].getY() + (m_aBlocks[i].getSize()-1)*int(m_aBlocks[i].getAxis()))*6+m_aBlocks[i].getX()+(m_aBlocks[i].getSize()-1)*(1-int(m_aBlocks[i].getAxis()))] = 1+i;
	}
}

/**
 *  Return wether a move is impossible
 */
template<unsigned int W, unsigned int H>
bool RushHour::Board<W,H>::is_move_impossible(int mvt, unsigned int num_vehicule)
{
	assert(num_vehicule < m_iCount);

	int cases_occ[W * H];

	get_parking_occupation(cases_occ);

	int newAbs;
	int newOrd;
	int largeur = (m_aBlocks[num_vehicule].getSize()-1) * (1 - m_aBlocks[num_vehicule].getAxis()); // largeur
	int hauteur = (m_aBlocks[num_vehicule].getSize()-1) * m_aBlocks[num_vehicule].getAxis(); // hauteur

	if(mvt != -1 && mvt != 1)
	{
		return true;
	}

	if (mvt == 1) { // 1 : droite ou bas
		newAbs = m_aBlocks[num_vehicule].getX() + (1 - m_aBlocks[num_vehicule].getAxis()); // horizontal
		newOrd = m_aBlocks[num_vehicule].getY() + m_aBlocks[num_vehicule].getAxis(); // vertical
	} else { // -1 : gauche ou haut
		newAbs = m_aBlocks[num_vehicule].getX() - (1 - m_aBlocks[num_vehicule].getAxis()); // horizontal
		newOrd = m_aBlocks[num_vehicule].getY() - m_aBlocks[num_vehicule].getAxis(); // vertical
	}

	//Autorisation de sortie du parking
	if (newAbs+largeur > 5 && newOrd == 2)
	{
		return false;
	}

	// On vérifie si la voiture est entierement dans le damier
	if (newAbs < 0 || newOrd < 0 || newAbs+largeur > 5 || newOrd+hauteur > 5)
	{
		return true;
	}

	// On vérifie si la case la plus en haut, ou la plus à gauche, est occupée
	if (mvt == -1 && cases_occ[newOrd*6 + newAbs] != 0)
	{
		return true;
	}

	// On vérifie si la case la plus en bas, ou la plus à droite, est occupée
	if (mvt == 1 && cases_occ[(newOrd+hauteur)*6 + (newAbs+largeur)] != 0)
	{
		return true;
	}

	return false;
}

/**
 * @brief RushHour::Board<W, H>::getMaxNegativeMove
 * @param index
 * @return
 */
template<unsigned int W, unsigned int H>
unsigned int RushHour::Board<W,H>::getMaxNegativeMove(unsigned int index) const
{
	assert(index < m_iCount);

	int cases_occ[W * H];
	get_parking_occupation(cases_occ);

	// Get Axis
	int axis = m_aBlocks[index].getAxis();

	// Top Left Position
	unsigned int x = m_aBlocks[index].getX();
	unsigned int y = m_aBlocks[index].getY();

	// Compute max move before going out
	unsigned int max = (Block::e_axis_horizontal == axis) ? x : y;

	// Check all move are possible
	for (unsigned int i = 1; i <= max; ++i)
	{
		unsigned int new_x = x - ((Block::e_axis_horizontal == axis) ? i : 0);
		unsigned int new_y = y - ((Block::e_axis_horizontal == axis) ? 0 : i);

		if (0 != cases_occ[new_y * W + new_x])
		{
			return(i-1);
		}
	}

	return(max);
}

/**
 * @brief RushHour::Board<W, H>::getMaxPositiveMove
 * @param index
 * @return
 */
template<unsigned int W, unsigned int H>
unsigned int RushHour::Board<W,H>::getMaxPositiveMove(unsigned int index) const
{
	assert(index < m_iCount);

	int cases_occ[W * H];
	get_parking_occupation(cases_occ);

	// Get Axis
	int axis = m_aBlocks[index].getAxis();

	// Bottom Right Position
	unsigned int x = m_aBlocks[index].getX() + (m_aBlocks[index].getSize() - 1) * (1 - axis);
	unsigned int y = m_aBlocks[index].getY() + (m_aBlocks[index].getSize() - 1) * axis;

	// Compute max move before going out
	unsigned int max = (Block::e_axis_horizontal == axis) ? (5-x) : (5-y);

	// special case : allow the main block to get out (victory)
	if (0 == index)
	{
		++max;
	}

	// Check all move are possible
	for (unsigned int i = 1; i <= max; ++i)
	{
		unsigned int new_x = x + ((Block::e_axis_horizontal == axis) ? i : 0);
		unsigned int new_y = y + ((Block::e_axis_horizontal == axis) ? 0 : i);

		if (new_x < W && new_y < H && 0 != cases_occ[new_y * W + new_x])
		{
			return(i-1);
		}
	}

	return(max);
}

/**
 * @brief RushHour::Board<W, H>::isCompleted
 * @return
 */
template<unsigned int W, unsigned int H>
bool RushHour::Board<W,H>::isCompleted(void) const
{
	return(5 == m_aBlocks[0].getX());
}

/**
 * @brief RushHour::Board<W, H>::getHardestConfiguration
 * @return
 */
template<unsigned int W, unsigned int H>
unsigned int RushHour::Board<W,H>::getHardestConfiguration(Board<W, H> & hardestConfig) const
{
	// Search Containers
	BinarySearchTree<Board> tree; // used to know which configuration has already been tested
	Queue<Board> queue; // used to track configuration to be tested

	//
	// Insert first node
	tree.insert(*this);
	queue.enqueue(*this);

	unsigned int max_distance = computeDistanceToSolution();
	hardestConfig = *this;

	while (!queue.empty())
	{
		const Board<W, H> & board = queue.first();

		for (int i = 0; i < m_iCount; i++) // for each block
		{
			//
			//
			const unsigned int negative = board.getMaxNegativeMove(i);

			if (negative >= 1)
			{
				Board<W, H> newBoard = board;
				newBoard.move(-1, i);

				bool added = tree.insert(newBoard);

				if (added)
				{
					unsigned int distance = newBoard.computeDistanceToSolution();

					queue.enqueue(newBoard);

					if (distance > max_distance)
					{
						max_distance = distance;
						hardestConfig = newBoard;
					}
				}
			}

			//
			//
			const unsigned int positive = board.getMaxPositiveMove(i);

			if (positive >= 1)
			{
				Board<W, H> newBoard = board;
				newBoard.move(+1, i);

				bool added = tree.insert(newBoard);

				if (added)
				{
					unsigned int distance = newBoard.computeDistanceToSolution();

					queue.enqueue(newBoard);

					if (distance > max_distance)
					{
						max_distance = distance;
						hardestConfig = newBoard;
					}
				}
			}
		}

		queue.dequeue();
	}

	return(max_distance);
}

/**
 * @brief RushHour::Board<W, H>::getPathToSolution
 * @return
 */
template<unsigned int W, unsigned int H>
bool RushHour::Board<W,H>::getPathToSolution(RushHour::Path & outputPath) const
{
	if (isCompleted())
	{
		return(0);
	}

	// Search Containers
	BinarySearchTree<Board> tree;
	Queue<BoardAndPath> queue;

	// insert initial node
	{
		BoardAndPath BP;
		BP.board = *this;
		queue.enqueue(BP);
	}

	// Insert current configuration in the tree
	tree.insert(*this);

	// Resolution
	while (!queue.empty())
	{
		const BoardAndPath & first = queue.first();

		for (int i = 0; i < m_iCount; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				Board			CurrentBoard	= first.board;
				const Path &	CurrentPath		= first.path;

				if (!CurrentBoard.is_move_impossible(2*j-1, i))
				{
					CurrentBoard.move(2*j-1, i);

					if (!CurrentBoard.isCompleted())
					{
						bool added = tree.insert(CurrentBoard);

						if (added)
						{
							const int length = CurrentPath.getCount();

							BoardAndPath BP;

							BP.board = CurrentBoard;

							BP.path.setCount(length + 1);

							for (int k = 0; k < length; ++k)
							{
								BP.path.setMove(k, CurrentPath.getMove(k));
							}

							BP.path.setMove(length, Move(i,j));

							queue.enqueue(BP);
						}
					}
					else
					{
						const int length = CurrentPath.getCount();

						outputPath.setCount(length + 1);

						for (int k = 0; k < length; ++k)
						{
							outputPath.setMove(k, CurrentPath.getMove(k));
						}

						outputPath.setMove(length, Move(i,j));

						return(true);
					}
				}
			}
		}

		queue.dequeue();
	}

	return(false);
}

/**
 * @brief RushHour::Board<W, H>::computeDistanceToSolution
 * @return
 */
template<unsigned int W, unsigned int H>
unsigned int RushHour::Board<W,H>::computeDistanceToSolution(void) const
{
	RushHour::Path pathToSolution;

	bool found = getPathToSolution(pathToSolution);

	if (!found)
	{
		return(0); // ???
	}

	return(pathToSolution.getCount());
}

// Force instantiation
template class RushHour::Board<6,6>;

