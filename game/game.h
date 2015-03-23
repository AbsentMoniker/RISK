#ifndef GAME_H
#define GAME_H
// game.h:
// Main game loop, game initialization, and game flow.
//
// Part of R.I.S.K
// ECE 477 Group 2, Spring 2015

#include "types.h"

// Game options. These are determined by setting options during game startup
// and do not change during the course of a game.
extern int randomTerritories;
extern int numPlayers;
extern int multipleDeploy;
extern CardValueScheme cardValueScheme;
#define OPTION_NUM_PLAYERS 0
#define OPTION_CARD_SCHEME 1
#define OPTION_RANDOM_TERRITORIES 2
#define OPTION_MULTIPLE_DEPLOY 3

// Global variables holding the current gamestate. These are all things that
// the display routines need to know about to output information to the LEDs,
// in addition to the territories array in gamedata.c
extern int currentPlayer;
extern int source;
extern int destination;
extern int attackerDice[3];
extern int defenderDice[2];

// gameInput is the main entry point into the game logic; this function calls
// one of the other functions below it to advance the game state.
void gameInput(Input input);
// cardInput is the other type of input, a command from the web server that
// the current player wishes to exchange cards. Returns false if for some
// reason the exchange could not actually happen.
int cardInput(int card1, int card2, int card3);
// updateText is called after every game update to update the text displayed on
// the LCD
void updateText();

// These functions actually perform the bulk of the game logic; there is one
// function for each possible GameState value, and each responds to the four
// possible inputs in some way.
void chooseOptions(Input input);
void selectTerritories(Input input);
void deployTroops(Input input);
void reinforce(Input input);
void declareAttack(Input input);
void declareAttackTarget(Input input);
void resolveBattle(Input input);
void conquerTerritory(Input input);
void moveTroops(Input input);
void moveTroopsTarget(Input input);
void moveTroopsNumber(Input input);
void gameOver(Input input);

// Helper functions for the game logic; these are functions that get called
// from many of the above update functions.
void moveSelection(int movesource, int direction, int (*predicate)(int));
void changeState(State state);

// Reset the game state (territories, cards, etc.) to their initial values, in
// preparation for starting a new game.
void resetGame();

// playerLiving is true if player owns at least one territory; dead players do
// not get turns
int playerLiving(int player);
// computeReinforcements includes the number of reinforcements a player get
// from territory count and continent bonuses; it does not include card
// exchanging.
int computeReinforcements(int player);

// Randomly allocate territories to the players, in effect skipping the SELECT
// state.
void allocateRandomTerritories();

#endif