#include "Board.h"

#include <stdlib.h>
#include <assert.h>

#include <BinarySearchTree.h>
#include <Queue.h>

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
	m_aBlocks[m_iCount] = block;
	++m_iCount;
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

struct BoardAndPath
{
	RushHour::Board<6,6> config;
	RushHour::t_chemin chemin;
};

template<unsigned int W, unsigned int H>
RushHour::t_chemin RushHour::Board<W,H>::solution(void) const
{
	// Search Containers
	BinarySearchTree<Board> arbre;
	Queue<BoardAndPath> file;

	// Insert first node
	BoardAndPath BP;
	BP.chemin = (t_mouvement *)malloc(sizeof(t_mouvement));
	BP.chemin->voiture = -1;
	BP.chemin->deplacement = 0; // to distinguish initialisation from the end of a path
	BP.config = *this;

	file.enqueue(BP);

	// Insert current configuration in the tree
	arbre.add(*this);

	// ...
	t_chemin res;
	bool continuer = true;

	// Resolution
	do
	{
		for (int i = 0; i < m_iCount && continuer; i++)
		{
			for (int j = 0; j < 2 && continuer; j++)
			{
				const BoardAndPath & first = file.first();
				Board	CurrentConfig	= first.config;
				t_chemin	CurrentPath		= first.chemin;

				if (!CurrentConfig.is_move_impossible(2*j-1, i)) // mouvement possible
				{
					if (i == 0 && j == 1 && CurrentConfig.m_aBlocks[0].getX()+1 == 5 && CurrentConfig.m_aBlocks[0].getY() == 2 )
					{
						continuer = false; // victoire (fin condition sortie boucle while)
					}
					else
					{
						CurrentConfig.move(2*j-1, i); // mouvement
					}
				}

				bool nouveau = arbre.add(CurrentConfig); //ajout(&arbre, aux->suivant->config);

				// n'existe pas déjà dans l'arbre
				if (nouveau && continuer)
				{
					/* Compte la longueur du chemin */

					int l = 0; // longueur reelle du chemin

					while (!(CurrentPath[l].voiture == -1))
					{
						++l;
					} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.


					/* Création d'un nouveau noeud */

					BoardAndPath BP;

					BP.config = CurrentConfig;

					BP.chemin = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); // longueur actuelle + ajout (1,-1)

					// Copie du chemin precedent
					for (int k = 0; k < l; k++)
					{
						BP.chemin[k].voiture = CurrentPath[k].voiture;
						BP.chemin[k].deplacement = CurrentPath[k].deplacement;
					}

					//ajout du nouveau deplacement
					BP.chemin[l].voiture = i;
					BP.chemin[l].deplacement = j;

					BP.chemin[l+1].voiture = -1;
					BP.chemin[l+1].deplacement = -1;

					file.enqueue(BP);
				}

				// Victoire
				if (!continuer)
				{
					int l = 0; // longueur reelle du chemin

					while (!(CurrentPath[l].voiture == -1))
					{
						++l;
					} //RMQ : ici je ne teste que voiture pour garder l=0 si on est ds le 1er cas.

					res = (t_mouvement *)malloc(sizeof(t_mouvement)*(l+2)); // longueur actuelle + ajout (1,-1)

					// Copie du chemin precedent
					for (int k = 0; k < l; k++)
					{
						res[k].voiture = CurrentPath[k].voiture;
						res[k].deplacement = CurrentPath[k].deplacement;
					}

					//ajout du nouveau deplacement
					res[l].voiture = i;
					res[l].deplacement = j;

					res[l+1].voiture = -1;
					res[l+1].deplacement = -1;
				}
			} // Fin boucle j
		} // Fin boucle i

		if (continuer)
		{
			file.dequeue();
		}

	} while (continuer);

	return res;
}

// Force instantiation
template class RushHour::Board<6,6>;

