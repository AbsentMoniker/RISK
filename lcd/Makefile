CFLAGS=-std=c99 -Wall -g -MMD 
CXXFLAGS=-std=c++11 -Wall -g -MMD 
OBJ= lcdemu.o characters.o
DEPS=$(OBJ:.o=.d)

lcdemu: $(OBJ)
	g++ -o $@ $^ -std=c++11 -lsfml-graphics -lsfml-window -lsfml-system

-include $(DEPS)
