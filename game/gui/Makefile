CFLAGS=-std=c99 -Wall -g -MMD -DUSE_FULL_MAP
CXXFLAGS=-std=c++11 -Wall -g -MMD -DUSE_FULL_MAP
OBJ= gui.o ../game.o ../gamedata.o ../territory.o ../cards.o ../log.o
DEPS=$(OBJ:.o=.d)

risk: $(OBJ)
	g++ -o $@ $^ -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f $(OBJ) $(DEPS) risk

-include $(DEPS)
