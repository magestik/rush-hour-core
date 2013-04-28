#ifndef LEVEL_H
#define LEVEL_H


void init_level_file(std::string filename);
void close_level_file();

CGameBoard *parse_level_line(char *line, int len);
CGameBoard *read_level_file();

extern CGameBoard *parking_actuel; // global

void load_next_level();

#endif
