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
#define LOG_REINFORCE 0
#define LOG_ATTACK 1
#define LOG_BATTLE 2
#define LOG_CONQUER 3
#define LOG_MOVE 4
#define LOG_CARD_GIVEN 5
#define LOG_CARD_EXCHANGE 6

typedef struct
{
    LogType type;
    unsigned player : 3;
    unsigned territory : 6;
    unsigned troops : 12;
} LogReinforce;

typedef struct
{
    LogType type;
    unsigned attackingPlayer : 3;
    unsigned defendingPlayer : 3;
    unsigned attackingTerritory : 6;
    unsigned defendingTerritory : 6;
} LogAttack;

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

typedef struct
{
    LogType type;
    unsigned attackingPlayer : 3;
    unsigned territory : 6;
    unsigned troops : 12;
} LogConquer;

typedef struct
{
    LogType type;
    unsigned player : 3;
    unsigned sourceTerritory : 6;
    unsigned destinationTerritory : 6;
    unsigned troops : 9;
} LogMove;

typedef struct
{
    LogType type;
    unsigned player : 3;
    unsigned cardtype : 2;
    signed territory : 7;
} LogCardGiven;

typedef struct
{
    LogType type;
    unsigned player : 3;
    unsigned cardtype1 : 2;
    unsigned cardtype2 : 2;
    unsigned cardtype3 : 2;
    unsigned troops : 12;
} LogCardExchange;

typedef union
{
    LogType type;
    LogReinforce reinforce;
    LogAttack attack;
    LogBattle battle;
    LogConquer conquer;
    LogMove move;
    LogCardGiven cardgiven;
    LogCardExchange exchange;
} LogEntry;
 
#endif
