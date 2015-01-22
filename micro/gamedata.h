#ifndef GAMEDATA_H
#define GAMEDATA_H
// gamedata.h:
// Game constants and territory graph layout.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "territory.h"
#define MAX_PLAYERS 5

// TODO: this is a simple map for debugging. Plug in the real map at some point.
#define T_ZEROLAND  0
#define T_ONELAND   1
#define T_TWOLAND   2
#define T_THREELAND 3
#define T_FOURLAND  4
#define T_FIVELAND  5
#define T_SIXLAND   6
#define T_SEVENLAND 7
#define NUM_TERRITORIES 8

extern Territory territories[NUM_TERRITORIES];


#endif
