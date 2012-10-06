#ifndef RUSH_H
#define RUSH_H

#include "structures.h"

#define MAX_VEHICULE 15

extern t_parking *parking_actuel; // global
extern int nb_coups;


void load_next_level();
void move(int mvt, int num_vehicule, t_parking parking);

int * tab_positions(t_parking parking);
int mvt_impossible(int mvt, int num_vehicule, t_parking parking);


#endif
