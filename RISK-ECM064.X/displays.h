/* 
 * File:   displays.h
 * Author: jacob
 *
 * Created on April 3, 2015, 1:01 PM
 */

#ifndef DISPLAYS_H
#define	DISPLAYS_H

#include "gamelogic.h"

#define COLOR_BLACK   0
#define COLOR_RED     1
#define COLOR_GREEN   2
#define COLOR_YELLOW  3
#define COLOR_BLUE    4
#define COLOR_MAGENTA 5
#define COLOR_CYAN    6
#define COLOR_WHITE   7

const int playerColors[MAX_PLAYERS];

#define T8_MSECS(ms) ((ms) * 1562)

#define CONTINENT_LED_UPPER_SHIFT 4
#define CONTINENT_LED_LOWER_SHIFT 1
#define TERRITORY_LED_SHIFT       1

static const int digits[10] = {
    // gfedcba
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111100, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01100111, // 9
};


// 6 continent LEDs (packed into 3 bytes)
// 5 dice displays
// 1 exchange value board (3 bytes)
// 42 territories (3 bytes each)
#define DISPLAY_DATA_LENGTH (3 + 5 + 3 + 3*NUM_TERRITORIES)

extern unsigned char displayData[DISPLAY_DATA_LENGTH];

void updateDisplayData();

void disableDisplays();
void enableDisplays();

void startNewFrame();
void startCardsColor(int player);

#endif	/* DISPLAYS_H */

