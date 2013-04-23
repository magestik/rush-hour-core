#include "pre.h"

#define ANIMATION_DURATION 0.5f

extern int id_cube;

CGameBoard *parking_actuel;

int resolution = 0;
t_chemin chemin;

float animation = 0.0f;

//
// INITIALIZE
//

/*static*/ void SimpleApplication::OnPreInitialize(void) {
	init_level_file("levels.data"); // default file name
}

/*static*/ void SimpleApplication::OnPostInitialize(void) {

}

//
// UPDATE
//

/*static*/ void SimpleApplication::OnPreUpdate(float delta) {
	float x, y;

	if (resolution > 0) {
		animation += delta / ANIMATION_DURATION;
	}

	for (unsigned int i = 0; i < parking_actuel->vehicules.size(); i++) {
		if(0 == i) {
			glColor3ub(255, 0, 0);
		} else {
			glColor3ub((i*154)%255, (i*142)%255, (i*45)%255);
		}

		glPushMatrix();
			y = parking_actuel->vehicules[i]->position.ord*-3 + 6 - (parking_actuel->vehicules[i]->axis * 3 * (parking_actuel->vehicules[i]->size - 1));
			x = parking_actuel->vehicules[i]->position.abs*3 - 9;
			glTranslatef(y, 0, x);

			x = parking_actuel->vehicules[i]->axis * parking_actuel->vehicules[i]->size * 1.5 + (1 - parking_actuel->vehicules[i]->axis) * 1.5;
			y = (1 - parking_actuel->vehicules[i]->axis) * parking_actuel->vehicules[i]->size * 1.5 + parking_actuel->vehicules[i]->axis * 1.5;
			glScalef(x, 1, y);

			glCallList(id_cube);
		glPopMatrix();

	}
}

/*static*/ void SimpleApplication::OnPostUpdate(float delta) {

	if (resolution > 0 && animation > 1.0f) {
		int k = resolution - 1;

		move(2*chemin[k].deplacement-1, chemin[k].voiture, *parking_actuel);

		if (chemin[k].deplacement == -1 && chemin[k].voiture == -1) {
			resolution = 0;
			load_next_level();
		} else {
			resolution++;
		}

		animation = 0.0f;
	}
}

//
// RELASE
//

/*static*/ void SimpleApplication::OnPreRelease(void) {
	close_level_file();
}

/*static*/ void SimpleApplication::OnPostRelease(void) {

}

//
// INPUTS
//

/*static*/ void SimpleApplication::OnTouchDown(int iTouch, float positionX, float positionY) {
	if (0 == iTouch) {
		if (0 == resolution) {
			chemin = parking_actuel->solution();
			resolution = 1;
		}
	}
}

/*static*/ void SimpleApplication::OnTouchUp(int iTouch, float positionX, float positionY) {

}

/*static*/ void SimpleApplication::OnTouchMove(int iTouch, float positionX, float positionY) {

}
