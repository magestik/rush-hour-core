#ifndef __PRE_H__
#define __PRE_H__

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>

typedef struct {
	short m_x;
	short m_y;
} t_position;


typedef struct {
	short voiture;
	short deplacement;
} t_mouvement;

typedef t_mouvement * t_chemin;

#define MAX_VEHICULE 15
#define HEIGHT 6
#define WIDTH 6

class SimpleApplication {

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



#include "GameBlock.h"
#include "GameBoard.h"

#include "BinarySearchTree.h"
#include "Queue.h"

#include "level.h"


#endif