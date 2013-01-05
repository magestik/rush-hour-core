CC=gcc
CFLAGS=-Wall
GLFLAGS=-lGL -lGLU -lglut -lm

TARGETS=BiG_Hour

all: $(TARGETS)

BiG_Hour: rush.o graphique.o resolution.o level.o
	$(CC) -o $@ $^ $(CFLAGS) $(GLFLAGS)

%.o: ./src/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGETS)
