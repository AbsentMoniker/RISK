CFLAGS=-std=c99 -Wall -g
CXXFLAGS=-std=c++11 -Wall -g
OBJ= gui.o ../game.o ../gamedata.o ../territory.o ../cards.o

risk: $(OBJ)
	g++ -o $@ $^ -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(OBJ) risk
