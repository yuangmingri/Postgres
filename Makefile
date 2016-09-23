CXX = g++
CXXFLAGS = --std=c++14 -Wall -W -Os
LDLIBS = -lm -lz -lpq -lrt -ldl


all: postgres


postgres: main.o postgress.o
	$(CXX) $(CXXFLAGS) *.o -o postgres $(LDLIBS)
	#rm -Rf *.o


main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	rm -Rf postgres *.o
