#ifndef IO_H
#define IO_H
// io.h:
// Interface from game logic to hardware inputs and outputs.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

// Functions here are not implemented by the game logic, because their
// implementation will depend on the system.

// Returns a random integer between min and max, inclusive.
int randint(int min, int max);
// Sets a line of the LCD display. If the string is longer than the maximum
// line length after formatting, the extra characters should be cut off.
void setTextDisplay(int line, const char * format, ...);

#endif
