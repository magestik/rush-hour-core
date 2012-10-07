#ifndef RUSH_H
#define RUSH_H

#include "structures.h"

#define MAX_VEHICULE 15

extern t_parking *parking_actuel; // global
extern int nb_coups;

void load_next_level();
void move(int mvt, int num_vehicule, t_parking parking);

void get_parking_occupation(t_parking parking, int *tableau);
int is_move_possible(int mvt, int num_vehicule, t_parking parking);


#endif
