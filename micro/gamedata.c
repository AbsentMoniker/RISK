// gamedata.c:
// Game constants and territory graph layout.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "gamedata.h"

Territory territories[NUM_TERRITORIES] =
{
    { "Zeroland",     { T_ONELAND, T_SEVENLAND, -1, -1, -1, -1},  INFANTRY,
        -1, 0},
    { "Oneland",      { T_TWOLAND, T_ZEROLAND, -1, -1, -1, -1},   INFANTRY,
        -1, 0},
    { "Twoland",      { T_THREELAND, T_ONELAND, -1, -1, -1, -1},  INFANTRY,
        -1, 0},
    { "Threeland",    { T_FOURLAND, T_TWOLAND, -1, -1, -1, -1},   INFANTRY,
        -1, 0},
    { "Fourland",     { T_FIVELAND, T_THREELAND, -1, -1, -1, -1}, INFANTRY,
        -1, 0},
    { "Fiveland",     { T_SIXLAND, T_FOURLAND, -1, -1, -1, -1},   INFANTRY,
        -1, 0},
    { "Sixland",      { T_SEVENLAND, T_FIVELAND, -1, -1, -1, -1}, INFANTRY,
        -1, 0},
    { "Sevenland",    { T_ZEROLAND, T_SIXLAND, -1, -1, -1, -1},   INFANTRY,
        -1, 0},
};

int cardExchangeValues[NUM_EXCHANGE_VALUES] = 
{ 4, 6, 8, 10, 12, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85,
    90, 95};

int initialTroops[MAX_PLAYERS + 1] = 
{ 0, 0, 40, 35, 30, 25, 20};
