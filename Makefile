CFLAGS=-Wall
GLFLAGS=-lGL -lGLU -lglut -lm

TARGETS=BiG_Hour

all: $(TARGETS)

BiG_Hour: rush.o graphique.o resolution.o level.o
	$(CXX) -o $@ $^ $(CFLAGS) $(GLFLAGS)

%.o: ./src/%.c
	$(CXX) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o

mrproper: clean
	rm -f $(TARGETS)
