#ifndef LEVEL_H
#define LEVEL_H

#include "rush.h"

#define FILE_SIZE_VEHICULE 8

void init_level_file(char *filename);
void close_level_file();

t_parking *parse_level_line(char *line, int len);
t_parking *read_level_file();

#endif
