#ifndef GAME_H
#define GAME_H
// game.h:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "types.h"

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

#endif
