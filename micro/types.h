#ifndef TYPES_H
#define TYPES_H
// types.h:
// Structures and enumerations.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "limits.h"
#include <stdint.h>

// Possible game states. See gamelogic.txt for details.
typedef enum
{
    INIT, SELECT, DEPLOY,  
    REINFORCE, ATTACK1, ATTACK2, BATTLE, CONQUER, MOVE1, MOVE2, MOVE3,
    GAMEOVER,
} State;

// Possible inputs to the game, corresponding to two buttons and two
// directions of the knob.
typedef enum
{
    ADVANCE, CANCEL, NEXT, PREVIOUS,
} Input;

// Types of cards.
typedef enum
{
    INFANTRY, CAVALRY, ARTILLERY, WILD,
} CardType;

// Possible schemes for card exchange values.
typedef enum
{
    INCREASING, INCREASING_ONE, SET_VALUE,
} CardValueScheme;

// Territory structure.
// The neighbors are not pointers to other structures, but simple indexes
// into the global array of territories.
typedef struct
{
    const char * name;
    const int neighbors[MAX_NEIGHBORS];
    const CardType cardtype;

    int owner;
    int troops;
} Territory;

// Continent structure
typedef struct
{
    const char * name;
    const int firstmember;
    const int members;
    const int value;
} Continent;

// Card structure
typedef struct
{
    CardType type;
    int territory;
} Card;
typedef struct
{
    Card hand[MAX_HAND_SIZE];
    int cards;
} Hand;

// Log structure for various game events.
typedef uint8_t LogType;
#define LOG_BATTLE 0

typedef struct
{
    LogType type;
    unsigned attackingPlayer : 3;
    unsigned defendingPlayer : 3;
    unsigned aDie1 : 3;
    unsigned aDie2 : 3;
    unsigned aDie3 : 3;
    unsigned dDie1 : 3;
    unsigned dDie2 : 3;
} LogBattle;

typedef union
{
    LogType type;
    LogBattle battle;
} LogEntry;
 
#endif
