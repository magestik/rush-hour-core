#ifndef LEVEL_H
#define LEVEL_H

#include "rush.h"

#include <string>

using namespace std;

void init_level_file(string filename);
void close_level_file();

t_parking *parse_level_line(char *line, int len);
t_parking *read_level_file();

#endif
