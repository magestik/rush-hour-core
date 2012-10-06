CC=gcc
CFLAGS=-Wall
GLFLAGS=-lGL -lGLU -lglut -lm

TARGETS=rush_console rush_graphique

all: $(TARGETS)

rush_console: rush_console.o console.o resolution.o level.o
	$(CC) -o $@ $^ $(CFLAGS)

rush_console.o: rush.c
	$(CC) -o $@ -c $< $(CFLAGS) -DINTERFACE_CONSOLE

rush_graphique: rush_graphique.o graphique.o resolution.o level.o
	$(CC) -o $@ $^ $(CFLAGS) $(GLFLAGS)

rush_graphique.o: rush.c
	$(CC) -o $@ -c $< $(CFLAGS)	-DINTERFACE_GRAPHIQUE

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGETS)
