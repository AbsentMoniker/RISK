// This file is part of R.I.S.K.
// Copyright 2015 Jacob Varnau, Cameron Young, Colin Graber, Jason Kohl
// See "COPYING" for details.
#ifndef GAMEDATA_H
#define GAMEDATA_H

#include "types.h"

// Symbolic names for each territory. Much of the game logic code assumes that
// these cover [0, NUM_TERRITORIES), and the continent structures assume that
// the each continent's members are consecutive numbers.
#ifdef USE_SIMPLE_MAP
#define T_ALPHA 0
#define T_BETA 1
#define T_GAMMA 2
#define T_DELTA 3
#else
#define T_ALASKA 6
#define T_NORTHWEST 7
#define T_GREENLAND 10
#define T_ALBERTA 8
#define T_ONTARIO 9
#define T_QUEBEC 11
#define T_WESTERN_UNITED_STATES 5
#define T_EASTERN_UNITED_STATES 12
#define T_MEXICO 4
#define T_VENEZUELA 3
#define T_PERU 2
#define T_ARGENTINA 0
#define T_BRAZIL 1
#define T_ICELAND 15
#define T_SCANDINAVIA 16
#define T_GREAT_BRITAIN 13
#define T_NORTHERN_EUROPE 14
#define T_WESTERN_EUROPE 40
#define T_SOUTHERN_EUROPE 39
#define T_UKRAINE 17
#define T_NORTHERN_AFRICA 41
#define T_EGYPT 37
#define T_CONGO 35
#define T_EASTERN_AFRICA 36
#define T_SOUTH_AFRICA 34
#define T_MADAGASCAR 33
#define T_URALS 18
#define T_SIBERIA 19
#define T_YAKUTSK 20
#define T_KAMCHATKA 21
#define T_IRKUTSK 22
#define T_AFGHANISTAN 25
#define T_JAPAN 23
#define T_MONGOLIA 24
#define T_MIDDLE_EAST 38
#define T_CHINA 27
#define T_INDIA 26
#define T_SIAM 28
#define T_INDONESIA 29
#define T_NEW_GUINEA 30
#define T_WESTERN_AUSTRALIA 32
#define T_EASTERN_AUSTRALIA 31

#endif

// Order in which territories should scroll through when a player is turning the
// selection knob.
extern int territoryScrollOrder[NUM_TERRITORIES];

// Array of every territory, including its current state. This can be thought
// of as a graph, since every territory knows it neighbors, but we never need
// to do any complex graph analysis or anything with this structure.
extern Territory territories[NUM_TERRITORIES];

// Array of continent information. These are simple structures, and assume that
// the territories that make up each continent are consecutive.
extern Continent continents[NUM_CONTINENTS];

// Array of card exchange values. According to the actual rules of Risk, the
// values should just keep increasing indefinitely, but since we only two
// digits, we might as well stop at 95.
// TODO: add support for alternate sequences (4,5,6,8...)
extern const int cardExchangeValues[NUM_EXCHANGE_VALUES];

// Array of the number of troops that each player start with, for a game with a
// given number of players. The first two values don't matter, since a 0-player
// or 1-player game don't make any sense.
extern const int initialTroops[MAX_PLAYERS + 1];

#endif
