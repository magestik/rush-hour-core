#ifndef RUSH_H
#define RUSH_H

#include "structures.h"

#define MAX_VEHICULE 15

extern t_parking *parking_actuel; // global
extern int nb_coups;

t_parking configuration_load(int num_config);
void configuration_next();

int * tab_positions(t_parking parking);
int mvt_impossible(int mvt, int num_vehicule, t_parking parking);
void bouger(int mvt, int num_vehicule, t_parking parking);

#endif
