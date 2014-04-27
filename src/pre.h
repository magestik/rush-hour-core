#ifndef __PRE_H__
#define __PRE_H__

/* STL */
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>

/* Own containers */

#include <BinarySearchTree.h>
#include <Queue.h>

/* Entry Point */

class SimpleApplication
{

public:
	static void OnPreInitialize		(void);
	static void OnPostInitialize	(void);

	static void	OnPreUpdate			(float delta);
	static void	OnPostUpdate		(float delta);

	static void OnPreRelease		(void);
	static void OnPostRelease		(void);

    static void OnTouchDown         (int iTouch, float positionX, float positionY);
    static void OnTouchUp           (int iTouch, float positionX, float positionY);
    static void OnTouchMove         (int iTouch, float positionX, float positionY);
};

struct t_position
{
	short m_x;
	short m_y;
};

#define MAX_VEHICULE 15
#define HEIGHT 6
#define WIDTH 6

#include "GamePath.h"

#include "GameBlock.h"
#include "GameBoard.h"

#include "level.h"

#endif
