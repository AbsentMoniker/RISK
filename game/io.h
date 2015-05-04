// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#ifndef IO_H
#define IO_H
// Functions here are not implemented by the game logic, because their
// implementation will depend on the system.

// Returns a random integer between min and max, inclusive.
int randint(int min, int max);
// Sets a line of the LCD display. If the string is longer than the maximum
// line length after formatting, the extra characters should be cut off.
void setTextDisplay(int line, const char * format, ...);

void panic(int line, const char * file, 
        const char * function, const char * text);
#define PANIC(text) panic(__LINE__, __BASE_FILE__, __func__, text)

void eraseSaveFile();

#endif
