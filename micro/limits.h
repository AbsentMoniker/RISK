#ifndef LIMITS_H
#define LIMITS_H
// limits.h:
// Game limits. These are defined separately from the rest of the game data
// because they are often needed in struct definitions.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

// Maximum number of players allowed (the minimum is always 2).
#define MAX_PLAYERS 6
// The most neighbors any one territory has.
#define MAX_NEIGHBORS 6
// The largest possible hand size (based on starting a turn with 4, then
// killing another player with 4)).
#define MAX_HAND_SIZE 8
// Number of territories/continenets on the standard board.
#define NUM_TERRITORIES 42
#define NUM_CONTINENTS 6
// Number of exchange values supported in the "increasing" sequence, before the
// value simply increases by 5 each time.
#define NUM_EXCHANGE_VALUES 6
// Number of cards in the risk deck
#define NUM_WILD_CARDS 2
#define NUM_CARDS (NUM_TERRITORIES + NUM_WILD_CARDS)
#endif
