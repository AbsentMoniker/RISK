#ifndef TYPES_H
#define TYPES_H
// types.h:
// Structures and enumerations.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

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

// Territory structure.
// The neighbors are not pointers to other structures, but simple indexes
// into the global array of territories.
#define MAX_NEIGHBORS 6
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
#endif
