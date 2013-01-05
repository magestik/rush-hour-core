#ifndef RESOLUTION_H
#define RESOLUTION_H

#include "rush.h"

int superieur(t_parking p1,t_parking p2);
int egal(t_parking p1, t_parking p2);
int ajout(t_ABR *pa, t_parking p);
void defiler(t_file *pF);

t_chemin solution(t_parking  parking);

#endif
