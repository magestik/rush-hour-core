#ifndef RUSH_H
#define RUSH_H

#define MAX_VEHICULE 15

extern CGameBoard *parking_actuel; // global

void load_next_level();
void move(int mvt, unsigned int num_vehicule, CGameBoard parking);

void get_parking_occupation(CGameBoard parking, int *tableau);
int is_move_possible(int mvt, int num_vehicule, CGameBoard parking);


#endif
