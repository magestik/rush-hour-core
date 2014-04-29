CXXFLAGS = -g -Wall -pg -Iinclude
LDFLAGS = -lGL -lGLU -lglut -lm -pg

TARGETS = BiG_Hour

SOURCES = $(wildcard src/*.cpp)
HEADER = ./src/pre.h

all: $(TARGETS)

BiG_Hour: $(SOURCES:cpp=o)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.cpp pre.h.gch
	$(CXX) $< -o $@ -c $(CXXFLAGS)

pre.h.gch: ./src/pre.h
	$(CXX) $< -o ./src/$@ $(CXXFLAGS)

clean:
	cd src; rm -f *.o pre.h.gch

mrproper: clean
	rm -f $(TARGETS)
