#ifndef CONSOLE_H
#define CONSOLE_H

#include "structures.h"

void console_init();
int console_resolution(t_parking parking);
int console_jouer(t_parking parking);

int console_menu(int choix_menu);
void console_affiche(t_parking parking);

void gagner();

#endif
