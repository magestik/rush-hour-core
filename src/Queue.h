#ifndef QUEUE_H
#define QUEUE_H

typedef struct sMaillon {
	CGameBoard config;
	t_chemin chemin;
	struct sMaillon *suivant;
} t_Maillon;

typedef t_Maillon * t_file;

void defiler(t_file *pF);
void free_file(t_file file);

#endif
