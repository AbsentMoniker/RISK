CFLAGS=-std=c99 -Wall
CXXFLAGS=-std=c++11 -Wall
OBJ= gui.o ../game.o ../gamedata.o ../territory.o

risk: $(OBJ)
	g++ -o $@ $^ -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm $(OBJ) risk
