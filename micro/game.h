#ifndef GAME_H
#define GAME_H
// game.h:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "territory.h"
#include "gamedata.h"
#include "io.h"

// Possible game states. See gamelogic.txt for details.
typedef enum
{
    INIT, SELECT, DEPLOY, 
    REINFORCE, ATTACK1, ATTACK2, BATTLE, MOVE1, MOVE2, MOVE3,
    GAMEOVER,
} State;
// Possible inputs to the game, corresponding to two buttons and two directions
// of the knob.
typedef enum
{
    ADVANCE, CANCEL, NEXT, PREVIOUS,
} Input;

extern int numPlayers;
extern int firstPlayer;
extern int currentPlayer;

extern int source;
extern int destination;

extern int attackerDice[3];
extern int defenderDice[2];

extern int nextCardTroops;
extern int numTroops;

void initializeGame();

void gameInput(Input input);
void updateText();

void choosePlayers(Input input);
void selectTerritories(Input input);
void deployTroops(Input input);
void reinforce(Input input);
void declareAttack(Input input);
void declareAttackTarget(Input input);
void resolveBattle(Input input);
void moveTroops(Input input);
void moveTroopsTarget(Input input);
void moveTroopsNumber(Input input);


int playerLiving(int player);
int computeReinforcements(int player);
int canAttack(int territory);
int isNeighbor(int territory1, int territory2);
void doBattle(int territoryA, int territoryD);

#endif
